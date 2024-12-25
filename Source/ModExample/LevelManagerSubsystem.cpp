// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManagerSubsystem.h"

void ULevelManagerSubsystem::RegisterLevel(const TSoftObjectPtr<UWorld>& Level)
{
	RegisteredLevels.AddUnique(Level);
	UE_LOG(LogTemp, Log, TEXT("Registering new level: %s"), *Level.ToString());
}

void ULevelManagerSubsystem::UnregisterLevel(const TSoftObjectPtr<UWorld>& Level)
{
	RegisteredLevels.Remove(Level);
	UE_LOG(LogTemp, Log, TEXT("Unregistering level: %s"), *Level.ToString());
}
