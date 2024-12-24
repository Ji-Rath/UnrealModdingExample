// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddLevel.h"

#include "LevelManagerSubsystem.h"

void UGameFeatureAction_AddLevel::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	for (auto& Level : Levels)
	{
		LevelManagerSubsystem = GameInstance->GetSubsystem<ULevelManagerSubsystem>();
		LevelManagerSubsystem->RegisterLevel(Level);
	}
}

void UGameFeatureAction_AddLevel::OnGameFeatureRegistering()
{
	StartGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, 
		&UGameFeatureAction_AddLevel::HandleGameInstanceStart);
}

void UGameFeatureAction_AddLevel::OnGameFeatureUnregistering()
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
