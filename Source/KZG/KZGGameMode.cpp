// Copyright Epic Games, Inc. All Rights Reserved.

#include "KZGGameMode.h"
#include "KZGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKZGGameMode::AKZGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
