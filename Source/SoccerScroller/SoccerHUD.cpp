#include "SoccerHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "Announcement.h"
#include "HighScore.h"
#include "MainMenu.h"

void ASoccerHUD::AddMainMenu()
{
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && MainMenuClass)
	{
		MainMenu = CreateWidget<UMainMenu>(PlayerController, MainMenuClass);
		MainMenu->AddToViewport();
	}
}

void ASoccerHUD::AddCharacterOverlay()
{
    APlayerController* PlayerController = GetOwningPlayerController();

    if (PlayerController && CharacterOverlayClass)
    {
        CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
        CharacterOverlay->AddToViewport();
    }
}

void ASoccerHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

void ASoccerHUD::AddHighScore()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && HighScoreClass)
	{
		HighScore = CreateWidget<UHighScore>(PlayerController, HighScoreClass);
		HighScore->AddToViewport();
	}
}
