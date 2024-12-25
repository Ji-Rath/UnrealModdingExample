// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddLevel.h"

#include "GameFeaturesSubsystem.h"
#include "LevelManagerSubsystem.h"

void UGameFeatureAction_AddLevel::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	for (auto& Level : Levels)
	{
		LevelManagerSubsystem = GameInstance->GetSubsystem<ULevelManagerSubsystem>();
		LevelManagerSubsystem->RegisterLevel(Level);
	}
}

void UGameFeatureAction_AddLevel::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	StartGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, 
		&UGameFeatureAction_AddLevel::HandleGameInstanceStart);

	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			HandleGameInstanceStart(WorldContext.World()->GetGameInstance());
		}
	}
}

void UGameFeatureAction_AddLevel::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(StartGameInstanceHandle);

	if (LevelManagerSubsystem)
	{
		for (auto& Level : Levels)
		{
			LevelManagerSubsystem->UnregisterLevel(Level);
		}
	}
}
