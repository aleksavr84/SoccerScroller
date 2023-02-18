// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SoccerScrollerGameMode.generated.h"

UCLASS(minimalapi)
class ASoccerScrollerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASoccerScrollerGameMode();
	bool CheckIfNotCollected(class ACharacter* PlayerCharacter);
	void PlayerScoreUpdate(class ASoccerScrollerCharacter* SoccerCharacter, class ASoccerPlayerController* SoccerController);
	void PlayerDeath(ASoccerScrollerCharacter* SoccerCharacter);
	void RestartLevel();

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 3.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Initialization")
	TSubclassOf<class ACollectable> CollectableClass;
};



