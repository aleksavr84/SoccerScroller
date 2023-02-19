#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SoccerHUD.generated.h"

UCLASS()
class SOCCERSCROLLER_API ASoccerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	// Main Menu
	UPROPERTY(EditAnywhere, Category = "Initialization")
	TSubclassOf<class UUserWidget> MainMenuClass;

	UPROPERTY()
	class UMainMenu* MainMenu;

	void AddMainMenu();

	// CharacterOverlay
	UPROPERTY(EditAnywhere, Category = "Initialization")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	void AddCharacterOverlay();

	// Announcements
	UPROPERTY(EditAnywhere, Category = "Initialization")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY()
	class UAnnouncement* Announcement;

	void AddAnnouncement();

	// HighScore
	UPROPERTY(EditAnywhere, Category = "Initialization")
	TSubclassOf<UUserWidget> HighScoreClass;

	UPROPERTY()
	class UHighScore* HighScore;

	void AddHighScore();
};
