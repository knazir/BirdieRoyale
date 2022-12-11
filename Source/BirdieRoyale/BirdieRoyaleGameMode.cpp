// Copyright Epic Games, Inc. All Rights Reserved.

#include "BirdieRoyaleGameMode.h"
#include "BirdieRoyaleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABirdieRoyaleGameMode::ABirdieRoyaleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
