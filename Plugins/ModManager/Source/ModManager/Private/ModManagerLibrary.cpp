// Fill out your copyright notice in the Description page of Project Settings.


#include "ModManagerLibrary.h"

#include "GameFeaturesSubsystem.h"
#include "IPlatformFilePak.h"
#include "Interfaces/IPluginManager.h"

DEFINE_LOG_CATEGORY(LogModManagerLibrary);

TArray<FString> UModManagerLibrary::GetPakFilesInDirectory(const FString& Path)
{
	TArray<FString> result;
	FString Type = "*.pak";
	IFileManager::Get().FindFilesRecursive(result, *Path, *Type, true,false);
	return result;
}

bool UModManagerLibrary::LoadPakFile(const FString& Path)
{
	auto PlatformFile = GetOrAddPakPlatform();
	FString PakName = FPaths::GetBaseFilename(Path);

	// Get platform path
	FString InputPath = Path;
	FPaths::MakePlatformFilename(InputPath);

	// Retrieve mount point
	FString MountPoint = GetMountPoint(Path);
	int StartIndex = MountPoint.Find("/", ESearchCase::IgnoreCase, ESearchDir::FromEnd, MountPoint.Len()-2);
	FString RootPath = MountPoint.Mid(StartIndex,MountPoint.Len());

	UE_LOG(LogModManagerLibrary, Log, TEXT("Attempting to register mount point %s to %s"), *(MountPoint+"Content/"), *RootPath);
	FPackageName::RegisterMountPoint(RootPath, MountPoint+"Content/");
	UE_LOG(LogModManagerLibrary, Log, TEXT("Attempting to mount %s to %s"), *InputPath, *MountPoint);
	return PlatformFile->Mount(*InputPath, 0, *MountPoint);
}

bool UModManagerLibrary::UnloadPakFile(const FString& Path)
{
	auto PlatformFile = GetOrAddPakPlatform();
	FString MountPoint = GetMountPoint(Path);
	FString PakName = FPaths::GetBaseFilename(Path);
	
	int StartIndex = MountPoint.Find("/", ESearchCase::IgnoreCase, ESearchDir::FromEnd, MountPoint.Len()-2);
	FString RootPath = MountPoint.Mid(StartIndex,MountPoint.Len());

	FString InputPath = Path;
	FPaths::MakePlatformFilename(InputPath);
	
	FPackageName::UnRegisterMountPoint(RootPath, MountPoint+"Content/");
	if (!PlatformFile->Unmount(*InputPath))
	{
		UE_LOG(LogModManagerLibrary, Log, TEXT("Unable to unmount %s"),*InputPath)
		return false;
	}
	return true;
}

TArray<FString> UModManagerLibrary::GetPluginsInDirectory(const FString& Path)
{
	TArray<FString> result;
	FString Type = "*.uplugin";
	IFileManager::Get().FindFilesRecursive(result, *Path, *Type, true,false);
	return result;
}

bool UModManagerLibrary::LoadPlugin(UObject* WorldContext, const FString& Path, bool bIsGameFeature)
{
	auto World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::Assert);
	auto PlatformFile = GetOrAddPakPlatform();
	FString PluginDirectory = FPaths::GetPath(Path);
	FString PluginName = FPaths::GetBaseFilename(Path);

	// Register plugin
	FText FailReason;
	if (!IPluginManager::Get().AddToPluginsList(Path, &FailReason))
	{
		UE_LOG(LogModManagerLibrary, Error, TEXT("Unable to add to plugin list: %s"), *FailReason.ToString())
		return false;
	}
		
	if (!IPluginManager::Get().MountExplicitlyLoadedPlugin(PluginName))
	{
		UE_LOG(LogModManagerLibrary, Error, TEXT("Unable to mount plugin %s"), *PluginName);
	}
		

	// Load pak files in plugin
	TArray<FString> PakFiles = GetPakFilesInDirectory(PluginDirectory);
	for (const auto& PakFile : PakFiles)
	{
		if (!LoadPakFile(PakFile))
			return false;
	}

	// Register with game feature subsystem
	if (bIsGameFeature)
	{
		if (auto GameFeaturesSubsystem = GEngine->GetEngineSubsystem<UGameFeaturesSubsystem>())
		{
			FString PluginLoad = GameFeaturesSubsystem->GetPluginURL_FileProtocol(Path);
			GameFeaturesSubsystem->LoadAndActivateGameFeaturePlugin(PluginLoad, FGameFeaturePluginLoadComplete());
			UE_LOG(LogModManagerLibrary, Log, TEXT("Loading Game Feature: %s"), *PluginLoad);
		}
	}
	return true;
}

bool UModManagerLibrary::UnloadPlugin(UObject* WorldContext, const FString& Path, bool bIsGameFeature)
{
	FString PluginDirectory = FPaths::GetPath(Path);
	FString MountPoint = GetMountPoint(Path);
	FString PluginName = FPaths::GetBaseFilename(Path);
	
	// Unload pak files in plugin
	TArray<FString> PakFiles = GetPakFilesInDirectory(PluginDirectory);
	for (const auto& PakFile : PakFiles)
	{
		if (!UnloadPakFile(PakFile))
			return false;
	}

	// Unload game feature
	if (bIsGameFeature)
	{
		if (auto GameFeaturesSubsystem = GEngine->GetEngineSubsystem<UGameFeaturesSubsystem>())
		{
			FString PluginLoad = GameFeaturesSubsystem->GetPluginURL_FileProtocol(Path);
			GameFeaturesSubsystem->UnloadGameFeaturePlugin(PluginLoad, FGameFeaturePluginUnloadComplete());
		}
	}
	
	return true;
}

FString UModManagerLibrary::GetMountPoint(const FString& Path)
{
	auto PlatformFile = GetOrAddPakPlatform();
	
	FString MountPoint = "";
	FPakFile* Pak = nullptr;
	TRefCountPtr<FPakFile> PakFile = new FPakFile(PlatformFile, *Path, false);
	Pak = PakFile.GetReference();
	if (Pak->IsValid())
	{
		return MountPoint = Pak->GetMountPoint();
	}
	return "";
}

FPakPlatformFile* UModManagerLibrary::GetOrAddPakPlatform()
{
	IPlatformFile* ExistingPlatformFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
	if(ExistingPlatformFile)
	{
		return static_cast<FPakPlatformFile*>(ExistingPlatformFile);
	}
	
	auto PakPlatformFile = new FPakPlatformFile();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PakPlatformFile->Initialize(&PlatformFile, TEXT(""));
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
	return PakPlatformFile;
}
