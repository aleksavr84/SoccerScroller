#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoccerPlayerController.generated.h"

UCLASS()
class SOCCERSCROLLER_API ASoccerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void ShowHideMainMenu(bool bIsShowing);

	void ShowHidePlayerHUD(bool bIsShowing);
	void SetHUDScore(float Score);

	void ShowHideSpeedBar(bool bIsShowing);
	void SetSpeedBar(float Speed, float MaxSpeed);

	void SetHUDAnnouncementCountdown(float CountdownTime);

	void SetHighScore(float Score);
	void ShowHideHighScore(bool bIsShowing);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ASoccerHUD* SoccerHUD;

	UPROPERTY()
	class ASoccerScrollerGameMode* SoccerGameMode;

	float HUDScore;

	void SetHUDTime();

	float WarmupTime = 0.f;
	float LevelStartingTime = 0.f;
	float TimeOfDeath = 0.f;
	float TimeLeft = 0.f;

	bool bShowCountdown = false;


public:
	FORCEINLINE void SetShowCountdown(bool Value) { bShowCountdown = Value;}
	FORCEINLINE bool GetShowCountdown() const { return bShowCountdown; }

	FORCEINLINE void SetTimeOfDeath(float Value) { TimeOfDeath = Value; }
};
