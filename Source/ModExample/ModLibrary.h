// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	 * Fetches all pak files within the Plugins folder of a project
	 * @return Array of all pak files contained within the Plugins folder
	 */
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetAllMods();
	
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
