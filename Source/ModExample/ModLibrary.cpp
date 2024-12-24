// Fill out your copyright notice in the Description page of Project Settings.


#include "ModLibrary.h"

#include "IPlatformFilePak.h"
#include "Interfaces/IPluginManager.h"

TArray<FString> UModLibrary::GetFilesRecursive(const FString& Path, const FString& files)
{
	TArray<FString> result;
	IFileManager::Get().FindFilesRecursive(result, *Path, *files, true,false);
	return result;
}

TArray<FString> UModLibrary::GetSubdirectories(const FString& Path)
{
	TArray<FString> result;
	IFileManager::Get().FindFiles(result, *Path, false,true);
	return result;
}

void UModLibrary::MountPlugin(const FString& Plugin)
{
	bool bMounted = IPluginManager::Get().MountExplicitlyLoadedPlugin(Plugin);
	UE_LOG(LogTemp, Display, TEXT("Mounted Plugin: %s %s"), *Plugin, bMounted ? TEXT("true") : TEXT("false"));
}

void UModLibrary::RegisterGameFeature(const FString& Path)
{
	GEngine->GetEngineSubsystem<UGameFeaturesSubsystem>()->LoadAndActivateGameFeaturePlugin(Path, FGameFeaturePluginLoadComplete());
}

void UModLibrary::AddPluginSearchPath(const FString& Path)
{
	IPluginManager::Get().AddPluginSearchPath(Path);
}

void UModLibrary::RefreshPlugins()
{
	IPluginManager::Get().RefreshPluginsList();
}

void UModLibrary::DebugPaks()
{
	TArray<FString> Output;
	auto PakPlatform = (FPakPlatformFile*)FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
	if (PakPlatform != nullptr)
	{
		TArray<FString> Paks;
		PakPlatform->GetMountedPakFilenames(Paks);
		for (const auto& Pak : Paks)
		{
			Output.Emplace(FString::Printf(TEXT("pak: %s"), *Pak));
		}
	}
	else
	{
		Output.Emplace("PakPlatformFile = nullptr");
	}
	FFileHelper::SaveStringArrayToFile(Output, *FPaths::Combine(FPaths::ProjectDir(), TEXT("paks.log")));
	for (auto& String : Output)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *String);
	}
	
}

void UModLibrary::DebugVirtualPaths()
{
	TArray<FString> Output;
	TArray<FString> RootPaths;
	FPackageName::QueryRootContentPaths(RootPaths);
	for (const auto& RootPath : RootPaths)
	{
		FString ContentPath;
		FPackageName::TryConvertLongPackageNameToFilename(RootPath, ContentPath);
		Output.Emplace(FString::Printf(TEXT("RootPath: %s    -->    %s"), *RootPath, *ContentPath));
	}
	FFileHelper::SaveStringArrayToFile(Output, *FPaths::Combine(FPaths::ProjectDir(), TEXT("paths.log")));
	for (auto& String : Output)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *String);
	}
}
