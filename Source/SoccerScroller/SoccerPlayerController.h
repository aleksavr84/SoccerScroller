#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SoccerPlayerController.generated.h"

UCLASS()
class SOCCERSCROLLER_API ASoccerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHUDScore(float Score);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ASoccerHUD* SoccerHUD;

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
