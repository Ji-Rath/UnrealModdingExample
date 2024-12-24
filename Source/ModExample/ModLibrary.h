// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeaturesSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MODEXAMPLE_API UModLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * Fetches all files within a path. By default returns all files, to filter by extension, for example use "*.pak"
	 * @return Array of all pak files contained within the Plugins folder
	 */
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetFilesRecursive(const FString& Path, const FString& files = "*");

	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetSubdirectories(const FString& Path);

	UFUNCTION(BlueprintCallable)
	static void MountPlugin(const FString& Plugin);

	UFUNCTION(BlueprintCallable)
	static void AddPluginSearchPath(const FString& Path);

	UFUNCTION(BlueprintCallable)
	static void RefreshPlugins();

	UFUNCTION(BlueprintCallable)
	static void RegisterGameFeature(const FString& Path);
	
	/**
	 * Debug currently mounted pak files
	 */
	UFUNCTION(BlueprintCallable)
	static void DebugPaks();

	/**
	 * Debug virtual paths. Mainly used for seeing if a pak file was correctly mounted and registered
	 */
	UFUNCTION(BlueprintCallable)
	static void DebugVirtualPaths();
};
