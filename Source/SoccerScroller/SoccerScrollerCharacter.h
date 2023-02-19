// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SoccerScrollerCharacter.generated.h"

//#define ECC_Ball ECollisionChannel::ECC_GameTraceChannel1

UCLASS(config=Game)
class ASoccerScrollerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LeftFootCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightFootCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BallCheckCollision;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** PauseMenu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;

	/** Retry Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RetryAction;

	/** Quit Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* QuitAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

public:
	ASoccerScrollerCharacter();
	
	void Death();

	UFUNCTION(BlueprintCallable)
	void SwitchToWalk();

	UFUNCTION(BlueprintCallable)
	void SwitchToRun();

	UFUNCTION(BlueprintCallable)
	void SwitchToShootingMode();

	UFUNCTION(BlueprintCallable)
	void PlayerOutOfPlayground();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void PauseActionPressed();

	void ShootActionPressed();
	void ShootActionReleased();

	void RetryActionPressed();
	void QuitActionPressed();
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

private:
	class ASoccerScrollerGameMode* SoccerGameMode;
	class ASoccerPlayerController* SoccerPlayerController;
	class ABall* Ball;

	// Movement
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float StartSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float EndSpeed = 500.f;

	bool bIsMovementEnabled = false;

	// MainMenu
	bool bIsMenuShowing = false;
	void ShowHideMainMenu(bool bIsShowing);

	// Shooting
	UPROPERTY(EditAnywhere, Category = "Initialization")
	class UAnimMontage* ShootMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float MinShootingSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float MaxShootingSpeed = 5500.f;

	void PlayShootMontage();
	void StartShooting();

	UFUNCTION(BlueprintCallable)
	void Shooting();

	UFUNCTION(BlueprintCallable)
	void ShootingFinished();

	FTimerHandle ShootChargeTimer;
	void StartShootChargeTimer();
	void ShootChargeTimerFinished();

	float ShootChargeTime = 3.f;
	bool bShootButtonPressed = false;
	float ShootStartTime = 0.f;
	float ShootEndTime = 0.f;
	float ShootingSpeed = 1.f;
	bool bIsShootingMode = false;
	bool bIsShooting = false;
	bool bShootFinished = false;
	
	void CalculateShootingSpeed();
	void ShootTheBall();

	// Collectable
	FTimerHandle CollectTimer;
	void StartCollectTimer();
	void CheckIsAllCollected();

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float CollectCheckDelay = 2.0f;
;
	// Start
	FTimerHandle WarmupTimer;
	void StartWarmupTimer();
	void WarmupTimerFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float StartDelay = 3.0f;

	bool bIsStart = false;
	bool bIsDead = false;
	void Dead();

	// Cooldown
	FTimerHandle CooldownTimer;
	void StartCooldownTimer();
	void CooldownTimerFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float CooldownTime = 3.0f;

	// Callbacks
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

