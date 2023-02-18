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

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	ASoccerScrollerCharacter();
	
	void Death();

	UFUNCTION(BlueprintCallable)
	void SwitchToWalk();

	UFUNCTION(BlueprintCallable)
	void SwitchToRun();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void ShootActionPressed();
	void ShootActionReleased();
			

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

	// Shooting
	float ShootingSpeed = 7500.f;

	// Collectable
	FTimerHandle CollectTimer;
	void StartCollectTimer();
	void CheckIsAllCollected();

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float CollectCheckDelay = 2.0f;
;
	// Restart
	FTimerHandle RestartTimer;
	void StartRestartTimer();
	void RestartTimerFinished();

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float RestartDelay = 3.0f;

	bool bIsDead = false;
	void Dead();

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

