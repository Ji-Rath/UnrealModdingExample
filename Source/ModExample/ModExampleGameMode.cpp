// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModExampleGameMode.h"
#include "ModExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AModExampleGameMode::AModExampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
