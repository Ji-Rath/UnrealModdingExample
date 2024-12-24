// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MODEXAMPLE_API ULevelManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void RegisterLevel(const TSoftObjectPtr<UWorld>& Level);

	UFUNCTION(BlueprintCallable)
	void UnregisterLevel(const TSoftObjectPtr<UWorld>& Level);

	UPROPERTY(BlueprintReadOnly)
	TArray<TSoftObjectPtr<UWorld>> RegisteredLevels;
};
