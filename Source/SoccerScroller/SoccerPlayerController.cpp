#include "SoccerPlayerController.h"
#include "CharacterOverlay.h"
#include "Announcement.h"
#include "Components/TextBlock.h"
#include "SoccerScrollerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SoccerHUD.h"

void ASoccerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SoccerHUD = SoccerHUD == nullptr ? Cast<ASoccerHUD>(GetHUD()) : SoccerHUD;
    ASoccerScrollerGameMode* SoccerGameMode = Cast<ASoccerScrollerGameMode>(UGameplayStatics::GetGameMode(this));

    if (SoccerGameMode)
    {
        WarmupTime = SoccerGameMode->WarmupTime;
    }

    if (SoccerHUD)
    {
        if (SoccerHUD->CharacterOverlay == nullptr) SoccerHUD->AddCharacterOverlay();
    }
}

void ASoccerPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetHUDTime();
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
