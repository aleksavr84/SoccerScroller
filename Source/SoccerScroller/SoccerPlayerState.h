// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SoccerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SOCCERSCROLLER_API ASoccerPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void AddToScore(float ScoreAmount);

private:
	UPROPERTY()
	class ASoccerScrollerCharacter* SoccerCharacter;

	UPROPERTY()
	class ASoccerPlayerController* SoccerPlayerController;
};
