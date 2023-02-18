// Fill out your copyright notice in the Description page of Project Settings.


#include "SoccerPlayerState.h"
#include "SoccerPlayerController.h"
#include "SoccerScrollerCharacter.h"

void ASoccerPlayerState::AddToScore(float ScoreAmount)
{
    SetScore(GetScore() + ScoreAmount);

    SoccerCharacter = SoccerCharacter == nullptr ? Cast<ASoccerScrollerCharacter>(GetPawn()) : SoccerCharacter;

    if (SoccerCharacter)
    {
        SoccerPlayerController = SoccerPlayerController == nullptr ? Cast<ASoccerPlayerController>(SoccerCharacter->Controller) : SoccerPlayerController;

        if (SoccerPlayerController)
        {
            SoccerPlayerController->SetHUDScore(GetScore());
        }
    }
}