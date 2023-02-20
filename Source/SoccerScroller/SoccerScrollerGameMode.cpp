// Copyright Epic Games, Inc. All Rights Reserved.

#include "SoccerScrollerGameMode.h"
#include "SoccerScrollerCharacter.h"
#include "SoccerPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SoccerPlayerController.h"
#include "Collectable.h"
#include "UObject/ConstructorHelpers.h"

ASoccerScrollerGameMode::ASoccerScrollerGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_ThirdPersonCharacterSoccer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ASoccerScrollerGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

bool ASoccerScrollerGameMode::CheckIfNotCollected(ACharacter* PlayerCharacter)
{
	TArray<AActor*> Collectables;

	UGameplayStatics::GetAllActorsOfClass(this, CollectableClass, Collectables);

	for (auto Collectable : Collectables)
	{
		if (Collectable->GetActorLocation().X < PlayerCharacter->GetActorLocation().X)
		{
			return true;
		}
	}

	return false;
}

void ASoccerScrollerGameMode::PlayerScoreUpdate(class ASoccerScrollerCharacter* SoccerCharacter, class ASoccerPlayerController* SoccerController, float ScoreToAdd)
{
	ASoccerPlayerState* SoccerPlayerState = SoccerController ? Cast<ASoccerPlayerState>(SoccerController->PlayerState) : nullptr;

	if (SoccerPlayerState)
	{
		SoccerPlayerState->AddToScore(ScoreToAdd);
	}
}

void ASoccerScrollerGameMode::PlayerDeath(ASoccerScrollerCharacter* SoccerCharacter)
{
	SoccerCharacter->Death();
}

void ASoccerScrollerGameMode::RestartLevel()
{
	UWorld* World = GetWorld();

	if (World)
	{
		bUseSeamlessTravel = false;
		World->ServerTravel(FString("/Game/Maps/SoccerMap?listen"));
	}
}

