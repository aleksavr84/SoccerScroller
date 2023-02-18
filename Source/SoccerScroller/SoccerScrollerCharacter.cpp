// Copyright Epic Games, Inc. All Rights Reserved.

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

ASoccerScrollerCharacter::ASoccerScrollerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 120.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASoccerScrollerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ASoccerScrollerCharacter::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ASoccerScrollerCharacter::ActorEndOverlap);

	SoccerPlayerController = SoccerPlayerController == nullptr ? Cast<ASoccerPlayerController>(Cast<APlayerController>(Controller)) : SoccerPlayerController;
	SoccerGameMode = SoccerGameMode == nullptr ? GetWorld()->GetAuthGameMode<ASoccerScrollerGameMode>() : SoccerGameMode;
	
	//Add Input Mapping Context	
	if (SoccerPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(SoccerPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Start to check if All the collectibles are collected
	StartCollectTimer();
}

void ASoccerScrollerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsDead)
	{
		AddMovementInput(FVector(1, 0, 0), 1.0f);
	}
}

void ASoccerScrollerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ASoccerScrollerCharacter::ShootActionPressed);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ASoccerScrollerCharacter::ShootActionReleased);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoccerScrollerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoccerScrollerCharacter::Look);
	}
}

void ASoccerScrollerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASoccerScrollerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASoccerScrollerCharacter::ShootActionPressed()
{

}

void ASoccerScrollerCharacter::ShootActionReleased()
{
	if (Ball)
	{
		FVector Direction = GetActorForwardVector(); //GetMesh()->GetForwardVector();

		Ball->SetBallPhysicsToShooting();

		Ball->GetBallMesh()->AddImpulse(Direction * ShootingSpeed, FName(), true);
	}
}

void ASoccerScrollerCharacter::SwitchToWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = StartSpeed;
}

void ASoccerScrollerCharacter::SwitchToRun()
{
	GetCharacterMovement()->MaxWalkSpeed = EndSpeed;
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
			Dead();
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
	}
}

void ASoccerScrollerCharacter::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<ABall>(OtherActor))
	{
		Dead();
	}
}

void ASoccerScrollerCharacter::Dead()
{
	bIsDead = true;
	GetCharacterMovement()->StopMovementImmediately();
	StartRestartTimer();

	if (SoccerPlayerController)
	{
		SoccerPlayerController->SetTimeOfDeath(GetWorld()->GetTimeSeconds());
	}
}

void ASoccerScrollerCharacter::StartRestartTimer()
{
	GetWorldTimerManager().SetTimer(
		RestartTimer,
		this,
		&ASoccerScrollerCharacter::RestartTimerFinished,
		RestartDelay
	);

	if (SoccerPlayerController)
	{
		SoccerPlayerController->SetShowCountdown(true);
	}
}

void ASoccerScrollerCharacter::RestartTimerFinished()
{
	if (SoccerPlayerController)
	{
		SoccerPlayerController->SetShowCountdown(false);
	}

	if (SoccerGameMode)
	{
		SoccerGameMode->PlayerDeath(this);
	}
}

void ASoccerScrollerCharacter::Death()
{
	Destroy();

	if (SoccerGameMode)
	{
		SoccerGameMode->RestartLevel();
	}
}








