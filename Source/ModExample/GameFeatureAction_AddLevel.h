// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"

#include "GameFeatureAction_AddLevel.generated.h"

class ULevelManagerSubsystem;
/**
 * 
 */
UCLASS()
class MODEXAMPLE_API UGameFeatureAction_AddLevel : public UGameFeatureAction
{
	GENERATED_BODY()

	void HandleGameInstanceStart(UGameInstance* GameInstance);
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UWorld>> Levels;

	UPROPERTY()
	ULevelManagerSubsystem* LevelManagerSubsystem;
	FDelegateHandle StartGameInstanceHandle;
};
