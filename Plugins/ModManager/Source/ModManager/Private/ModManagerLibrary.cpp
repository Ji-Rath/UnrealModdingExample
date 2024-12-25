// Fill out your copyright notice in the Description page of Project Settings.


#include "ModManagerLibrary.h"

#include "GameFeaturesSubsystem.h"
#include "IPlatformFilePak.h"
#include "Interfaces/IPluginManager.h"

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
	FString RootPath = "/"+PakName+"/";

	// Get platform path
	FString InputPath = Path;
	FPaths::MakePlatformFilename(InputPath);

	// Retrieve mount point
	FString MountPoint = "";
	FPakFile* Pak = nullptr;
	TRefCountPtr<FPakFile> PakFile = new FPakFile(PakPlatformFile, *Path, false);
	Pak = PakFile.GetReference();
	if (Pak->IsValid())
	{
		MountPoint = Pak->GetMountPoint();
	}

	FPackageName::RegisterMountPoint(RootPath, MountPoint+"Content/");
	return PlatformFile->Mount(*InputPath, 0, *MountPoint);
}

bool UModManagerLibrary::UnloadPakFile(const FString& Path)
{
	auto PlatformFile = GetOrAddPakPlatform();
	FString PakName = FPaths::GetBaseFilename(Path);
	FString RootPath = "/"+PakName+"/";

	// Retrieve mount point
	FString MountPoint = "";
	FPakFile* Pak = nullptr;
	TRefCountPtr<FPakFile> PakFile = new FPakFile(PakPlatformFile, *Path, false);
	Pak = PakFile.GetReference();
	if (Pak->IsValid())
	{
		MountPoint = Pak->GetMountPoint();
	}
	
	FPackageName::UnRegisterMountPoint(RootPath, MountPoint+"Content/");
	return PlatformFile->Unmount(*Path);
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
		return false;
	if (!IPluginManager::Get().MountExplicitlyLoadedPlugin(PluginName))
		return false;

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
			GameFeaturesSubsystem->LoadAndActivateGameFeaturePlugin(Path, FGameFeaturePluginLoadComplete());
		}
	}
	return true;
}

bool UModManagerLibrary::UnloadPlugin(UObject* WorldContext, const FString& Path, bool bIsGameFeature)
{
	FString PluginDirectory = FPaths::GetPath(Path);
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
			GameFeaturesSubsystem->UnloadGameFeaturePlugin(Path, FGameFeaturePluginUnloadComplete());
		}
	}
	
	return true;
}

FPakPlatformFile* UModManagerLibrary::GetOrAddPakPlatform()
{
	IPlatformFile* ExistingPlatformFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
	if(ExistingPlatformFile)
	{
		PakPlatformFile = static_cast<FPakPlatformFile*>(ExistingPlatformFile);
	}
	else
	{
		PakPlatformFile = new FPakPlatformFile();
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		PakPlatformFile->Initialize(&PlatformFile, TEXT(""));
		FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
	}
	return PakPlatformFile;
}
