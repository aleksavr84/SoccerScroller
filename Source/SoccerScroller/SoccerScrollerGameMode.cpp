// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoccerScrollerGameMode.h"
#include "SoccerScrollerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoccerScrollerGameMode::ASoccerScrollerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
