// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManagerSubsystem.h"

void ULevelManagerSubsystem::RegisterLevel(const TSoftObjectPtr<UWorld>& Level)
{
	RegisteredLevels.AddUnique(Level);
}

void ULevelManagerSubsystem::UnregisterLevel(const TSoftObjectPtr<UWorld>& Level)
{
	RegisteredLevels.Remove(Level);
}
