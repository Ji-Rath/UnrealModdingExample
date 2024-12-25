// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModManagerLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogModManagerLibrary, Log, All);

class FPakPlatformFile;
/**
 * 
 */
UCLASS()
class MODMANAGER_API UModManagerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Retrieves all pak files recursively in the directory
	UFUNCTION(BlueprintCallable, Category = "Mod Manager | Pak File Modding")
	static TArray<FString> GetPakFilesInDirectory(const FString& Path);

	// Mounts and registers a pak file for use.
	UFUNCTION(BlueprintCallable, Category = "Mod Manager | Pak File Modding")
	static bool LoadPakFile(const FString& Path);

	// Unmounts a pak file
	UFUNCTION(BlueprintCallable, Category = "Mod Manager | Pak File Modding")
	static bool UnloadPakFile(const FString& Path);

	// Retrieves all .uplugin files recursively in the directory
	UFUNCTION(BlueprintCallable, Category = "Mod Manager | Plugin File Modding")
	static TArray<FString> GetPluginsInDirectory(const FString& Path);

	// Load a plugin for use
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContext"), Category = "Mod Manager | Plugin File Modding")
	static bool LoadPlugin(UObject* WorldContext, const FString& Path, bool bIsGameFeature);

	// Unload a plugin
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContext"), Category = "Mod Manager | Plugin File Modding")
	static bool UnloadPlugin(UObject* WorldContext, const FString& Path, bool bIsGameFeature);
protected:
	static FString GetMountPoint(const FString& Path);
	static FPakPlatformFile* GetOrAddPakPlatform();
};
