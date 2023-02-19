#include "SoccerPlayerController.h"
#include "CharacterOverlay.h"
#include "Announcement.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "SoccerScrollerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SoccerHUD.h"
#include "HighScore.h"
#include "MainMenu.h"

void ASoccerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SoccerHUD = SoccerHUD == nullptr ? Cast<ASoccerHUD>(GetHUD()) : SoccerHUD;
    SoccerGameMode = Cast<ASoccerScrollerGameMode>(UGameplayStatics::GetGameMode(this));

    if (SoccerGameMode)
    {
        WarmupTime = SoccerGameMode->WarmupTime;
    }

    if (SoccerHUD)
    {
        SoccerHUD->AddHighScore();
    }
}

void ASoccerPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetHUDTime();
}

void ASoccerPlayerController::ShowHideMainMenu(bool bIsShowing)
{
    if (SoccerHUD)
    {
        if (bIsShowing)
        {
            SoccerHUD->AddMainMenu();
            
        }
        else if (!bIsShowing && SoccerHUD->MainMenu)
        {
            SoccerHUD->MainMenu->RemoveFromParent();
        }
    }
}

void ASoccerPlayerController::SetHUDTime()
{
    if (bShowCountdown)
    {
        TimeLeft = WarmupTime - (GetWorld()->GetTimeSeconds() - TimeOfDeath);
        uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

        if (SoccerHUD)
        {
            if (SoccerHUD->Announcement == nullptr) SoccerHUD->AddAnnouncement();

            SetHUDAnnouncementCountdown(SecondsLeft);                
        }
    }
    else
    {
        if (SoccerHUD->Announcement)
        {
            SoccerHUD->Announcement->RemoveFromParent();
        }
    }
}

void ASoccerPlayerController::ShowHidePlayerHUD(bool bIsShowing)
{
    if (SoccerHUD)
    {
        if (bIsShowing)
        {
            if (SoccerHUD->CharacterOverlay == nullptr) SoccerHUD->AddCharacterOverlay();
        }
        else if (!bIsShowing && SoccerHUD->CharacterOverlay)
        {
            SoccerHUD->CharacterOverlay->RemoveFromParent();
        }
    }
}

void ASoccerPlayerController::SetHUDScore(float Score)
{
    SoccerHUD = SoccerHUD == nullptr ? Cast<ASoccerHUD>(GetHUD()) : SoccerHUD;

    if (SoccerHUD &&
        SoccerHUD->CharacterOverlay &&
        SoccerHUD->CharacterOverlay->ScoreAmount)
    {
        FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
        SoccerHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
    }
}

void ASoccerPlayerController::ShowHideSpeedBar(bool bIsShowing)
{
    if (SoccerHUD &&
        SoccerHUD->CharacterOverlay &&
        SoccerHUD->CharacterOverlay->SpeedBar)
    {
        if (bIsShowing)
        {
            SoccerHUD->CharacterOverlay->SpeedBar->SetVisibility(ESlateVisibility::Visible);
        }
        else if (!bIsShowing)
        {
            SoccerHUD->CharacterOverlay->SpeedBar->SetVisibility(ESlateVisibility::Hidden);            
        }
    }
}

void ASoccerPlayerController::SetSpeedBar(float Speed, float MaxSpeed)
{
    SoccerHUD = SoccerHUD == nullptr ? Cast<ASoccerHUD>(GetHUD()) : SoccerHUD;

    if (SoccerHUD &&
        SoccerHUD->CharacterOverlay &&
        SoccerHUD->CharacterOverlay->SpeedBar)
    {
        const float SpeedPercent = Speed / MaxSpeed;
        SoccerHUD->CharacterOverlay->SpeedBar->SetPercent(SpeedPercent);
    }
}

void ASoccerPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
    SoccerHUD = SoccerHUD == nullptr ? Cast<ASoccerHUD>(GetHUD()) : SoccerHUD;
    
    if (SoccerHUD &&
        SoccerHUD->Announcement &&
        SoccerHUD->Announcement->WarmupTime)
    {
        if (CountdownTime < 0.f)
        {
            SoccerHUD->Announcement->WarmupTime->SetText(FText());
            return;
        }

        int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
        int32 Seconds = CountdownTime - Minutes * 60;

        FString CountdownText = FString::Printf(TEXT("%d"), Seconds);
        SoccerHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
    }
}

void ASoccerPlayerController::SetHighScore(float Score)
{
    SoccerHUD = SoccerHUD == nullptr ? Cast<ASoccerHUD>(GetHUD()) : SoccerHUD;

    if (SoccerHUD &&
        SoccerHUD->HighScore &&
        SoccerHUD->HighScore->HighScoreValue)
    {
        FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
        SoccerHUD->HighScore->HighScoreValue->SetText(FText::FromString(ScoreText));
    }
}

void ASoccerPlayerController::ShowHideHighScore(bool bIsShowing)
{
    if (SoccerHUD &&
        SoccerHUD->HighScore &&
        SoccerHUD->HighScore->HighScoreText &&
        SoccerHUD->HighScore->HighScoreValue)
    {
        if (bIsShowing)
        {
            SoccerHUD->HighScore->HighScoreText->SetVisibility(ESlateVisibility::Visible);
            SoccerHUD->HighScore->HighScoreValue->SetVisibility(ESlateVisibility::Visible);
        }
        else if (!bIsShowing)
        {
            SoccerHUD->HighScore->HighScoreText->SetVisibility(ESlateVisibility::Hidden);
            SoccerHUD->HighScore->HighScoreValue->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}
