#include "SoccerScrollerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "SoccerPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"
#include "SoccerScrollerGameMode.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Ball.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"

ASoccerScrollerCharacter::ASoccerScrollerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 120.0f, 0.0f); 
	GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f; 	
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	// Left Foot
	LeftFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftFootCollision"));
	LeftFootCollision->SetupAttachment(GetMesh(), FName("foot_l"));
	LeftFootCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeftFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	LeftFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	LeftFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	LeftFootCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftFootCollision->CanCharacterStepUp(false);

	// Right Foot
	RightFootCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightFootCollision"));
	RightFootCollision->SetupAttachment(GetMesh(), FName("foot_r"));
	RightFootCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	RightFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	RightFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	RightFootCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightFootCollision->CanCharacterStepUp(false);

	// Ball Check
	BallCheckCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BallCheckCollision"));
	BallCheckCollision->SetupAttachment(GetMesh());
	BallCheckCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BallCheckCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallCheckCollision->CanCharacterStepUp(false);
}

void ASoccerScrollerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ASoccerScrollerCharacter::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ASoccerScrollerCharacter::ActorEndOverlap);

	SoccerPlayerController = SoccerPlayerController == nullptr ? Cast<ASoccerPlayerController>(Cast<APlayerController>(Controller)) : SoccerPlayerController;
	SoccerGameMode = SoccerGameMode == nullptr ? GetWorld()->GetAuthGameMode<ASoccerScrollerGameMode>() : SoccerGameMode;
	
	if (SoccerPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(SoccerPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Start to check if All the collectibles are collected
	StartCollectTimer();
	StartWarmupTimer();
}

void ASoccerScrollerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Player moves forwards automaticly 
	if (!bIsDead &&
		bIsStart &&
		!bShootFinished &&
		!bIsShooting)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 1.0f);
	}

	// Showing Shoot Power Indicator
	if (bShootButtonPressed)
	{
		if (SoccerPlayerController)
		{
			ShootEndTime = GetWorld()->GetTimeSeconds();
			CalculateShootingSpeedAndAngle();

			SoccerPlayerController->SetSpeedBar(ShootingSpeed, MaxShootingSpeed);
		}
	}
}

void ASoccerScrollerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//PauseMenu
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASoccerScrollerCharacter::PauseActionPressed);

		//Retry
		EnhancedInputComponent->BindAction(RetryAction, ETriggerEvent::Triggered, this, &ASoccerScrollerCharacter::RetryActionPressed);

		//Quit
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Triggered, this, &ASoccerScrollerCharacter::QuitActionPressed);
			
		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ASoccerScrollerCharacter::ShootActionPressed);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ASoccerScrollerCharacter::ShootActionReleased);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoccerScrollerCharacter::Move);
	}
}

void ASoccerScrollerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && bIsMovementEnabled)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (!bIsDead &&
			bIsStart &&
			!bShootFinished
			&& !bIsShooting)
		{
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ASoccerScrollerCharacter::PauseActionPressed()
{
	if (!bIsMenuShowing)
	{
		ShowHideMainMenu(true);

	}
	else if (bIsMenuShowing && !bShootFinished)
	{	
		ShowHideMainMenu(false);
	}
}

void ASoccerScrollerCharacter::ShowHideMainMenu(bool bIsShowing)
{
	if (SoccerPlayerController)
	{
		SoccerPlayerController->ShowHideMainMenu(bIsShowing);
		bIsMenuShowing = bIsShowing;
		SoccerPlayerController->SetPause(bIsShowing);
	}
}

void ASoccerScrollerCharacter::RetryActionPressed()
{
	if (bIsMenuShowing)
	{
		// Restarting the level
		Death();
	}
}

void ASoccerScrollerCharacter::QuitActionPressed()
{
	UWorld* World = GetWorld();

	if (bIsMenuShowing && World && SoccerPlayerController)
	{
		UKismetSystemLibrary::QuitGame(
			World,
			SoccerPlayerController,
			EQuitPreference::Quit,
			true
		);
	}
}

void ASoccerScrollerCharacter::StartWarmupTimer()
{
	GetWorldTimerManager().SetTimer(
		WarmupTimer,
		this,
		&ASoccerScrollerCharacter::WarmupTimerFinished,
		StartDelay
	);

	if (SoccerPlayerController)
	{
		bIsMovementEnabled = false;
		SoccerPlayerController->SetShowCountdown(true);
		SoccerPlayerController->ShowHideHighScore(false);
	}
}

void ASoccerScrollerCharacter::WarmupTimerFinished()
{
	if (SoccerPlayerController)
	{
		SoccerPlayerController->SetShowCountdown(false);
		SoccerPlayerController->ShowHidePlayerHUD(true);

		bIsStart = true;
		bIsMovementEnabled = true;
	}
}

void ASoccerScrollerCharacter::StartCollectTimer()
{
	GetWorldTimerManager().SetTimer(
		CollectTimer,
		this,
		&ASoccerScrollerCharacter::CheckIsAllCollected,
		CollectCheckDelay
	);
}

void ASoccerScrollerCharacter::CheckIsAllCollected()
{
	if (SoccerGameMode)
	{
		if (SoccerGameMode->CheckIfNotCollected(this))
		{
			bIsMovementEnabled = false;
			bIsDead = true;
			StartCooldownTimer();
		}
	}

	if (!bIsDead)
	{
		StartCollectTimer();
	}
}

void ASoccerScrollerCharacter::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ABall>(OtherActor))
	{
		Ball = Cast<ABall>(OtherActor);
		Ball->SetOwner(this);
	}
}

void ASoccerScrollerCharacter::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ABall>(OtherActor) && !bIsShootingMode)
	{
		bIsMovementEnabled = false;
		bIsDead = true;
		StartCooldownTimer();
	}
}

void ASoccerScrollerCharacter::PlayerOutOfPlayground()
{
	bIsMovementEnabled = false;
	bIsDead = true;
	StartCooldownTimer();
}

void ASoccerScrollerCharacter::SwitchToWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
}

void ASoccerScrollerCharacter::SwitchToRun()
{
	GetCharacterMovement()->MaxWalkSpeed = EndSpeed;
}

void ASoccerScrollerCharacter::SwitchToShootingMode()
{
	bIsShootingMode = true;
}

void ASoccerScrollerCharacter::ShootActionPressed()
{
	if (bIsShootingMode && bIsMovementEnabled)
	{
		bShootButtonPressed = true;
		StartShootChargeTimer();
		SoccerPlayerController->ShowHideSpeedBar(true);
	}
}

void ASoccerScrollerCharacter::StartShootChargeTimer()
{
	ShootStartTime = GetWorld()->GetTimeSeconds();

	GetWorldTimerManager().SetTimer(
		ShootChargeTimer,
		this,
		&ASoccerScrollerCharacter::ShootChargeTimerFinished,
		ShootChargeTime
	);
}

void ASoccerScrollerCharacter::ShootChargeTimerFinished()
{
	if (bShootButtonPressed)
	{
		// Restarting the Timer, when shoot button is still pressed
		StartShootChargeTimer();
	}
}

void ASoccerScrollerCharacter::ShootActionReleased()
{
	if (bIsShootingMode && bShootButtonPressed)
	{
		StartShooting();
		bShootButtonPressed = false;
	}
}

void ASoccerScrollerCharacter::StartShooting()
{
	if (Ball)
	{
		bIsShooting = true;
		bIsMovementEnabled = false;
		
		LeftFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		RightFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

		PlayShootMontage();
	}
}

void ASoccerScrollerCharacter::PlayShootMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && ShootMontage)
	{
		AnimInstance->Montage_Play(ShootMontage);
	}
}

void ASoccerScrollerCharacter::CalculateShootingSpeedAndAngle()
{
	ShootingSpeed = UKismetMathLibrary::MapRangeClamped(
		ShootEndTime - ShootStartTime,
		0.f,
		ShootChargeTime,
		MinShootingSpeed,
		MaxShootingSpeed
	);

	ShootingAngle = UKismetMathLibrary::MapRangeClamped(
		ShootEndTime - ShootStartTime,
		0.f,
		ShootChargeTime,
		MinShootingAngle,
		MaxShootingAngle
	);
}

// From AnimNotify
void ASoccerScrollerCharacter::Shooting()
{
	if (Ball)
	{
		Ball->DisablePhysics(false);
		ShootTheBall();
	}
}

void ASoccerScrollerCharacter::ShootTheBall()
{
	if (Ball)
	{
		if (SoccerPlayerController)
		{
			SoccerPlayerController->SetSpeedBar(ShootingSpeed, MaxShootingSpeed);
		}

		FVector Direction = GetArrowComponent()->GetForwardVector();
		
		Direction = FVector(Direction.X, Direction.Y, ShootingAngle);

		Ball->SetBallPhysicsToShooting();
		Ball->GetBallMesh()->AddImpulse(Direction * ShootingSpeed, FName(), true);
	}
}

// From AnimNotify
void ASoccerScrollerCharacter::ShootingFinished()
{
	if (Ball)
	{		
		bIsShooting = false;
		bShootFinished = true;

		LeftFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		RightFootCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

		SoccerPlayerController->ShowHideSpeedBar(false);
		StartCooldownTimer();
	}
}

void ASoccerScrollerCharacter::Dead()
{
	bIsDead = true;
	bIsMovementEnabled = false;

	if (SoccerGameMode)
	{
		SoccerGameMode->PlayerDeath(this);
	}
}

void ASoccerScrollerCharacter::StartCooldownTimer()
{
	GetWorldTimerManager().SetTimer(
		CooldownTimer,
		this,
		&ASoccerScrollerCharacter::CooldownTimerFinished,
		CooldownTime
	);

	if (SoccerPlayerController)
	{
		SoccerPlayerController->ShowHideHighScore(true);
		SoccerPlayerController->ShowHidePlayerHUD(false);
	}
}

void ASoccerScrollerCharacter::CooldownTimerFinished()
{
	ShowHideMainMenu(true);
}

void ASoccerScrollerCharacter::Death()
{
	Destroy();

	if (SoccerGameMode)
	{
		SoccerGameMode->RestartLevel();
	}
}








