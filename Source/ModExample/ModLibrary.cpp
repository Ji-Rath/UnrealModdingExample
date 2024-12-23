// Fill out your copyright notice in the Description page of Project Settings.


#include "ModLibrary.h"

#include "IPlatformFilePak.h"
#include "HAL/FileManagerGeneric.h"

TArray<FString> UModLibrary::GetAllMods()
{
	TArray<FString> result;
	FString path = FPaths::ProjectPluginsDir();
	FString files = "*.pak";
	IFileManager::Get().FindFilesRecursive(result, *path, *files, true,false);
	return result;
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
