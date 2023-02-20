#include "Cage.h"
#include "Components/BoxComponent.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "SoccerScrollerCharacter.h"
#include "SoccerScrollerGameMode.h"
#include "SoccerPlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"

ACage::ACage()
{
 	PrimaryActorTick.bCanEverTick = true;

	CageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CageMesh"));
	CageMesh->SetupAttachment(RootComponent);

	// Ball Check
	BallCheckCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BallCheckCollision"));
	BallCheckCollision->SetupAttachment(RootComponent);
	BallCheckCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BallCheckCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallCheckCollision->CanCharacterStepUp(false);

	HitEffectComponentLeft = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitEffectComponentLeft"));
	HitEffectComponentLeft->SetupAttachment(RootComponent);

	HitEffectComponentRight = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HitEffectComponentRight"));
	HitEffectComponentRight->SetupAttachment(RootComponent);
}

void ACage::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ACage::ActorBeginOverlap);
}

void ACage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACage::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ABall* Ball = Cast<ABall>(OtherActor);

	if (Ball && !bHitDetected)
	{
		ASoccerScrollerCharacter* SoccerCharacter = Cast<ASoccerScrollerCharacter>(Ball->GetOwner());
		ASoccerScrollerGameMode* SoccerGameMode = GetWorld()->GetAuthGameMode<ASoccerScrollerGameMode>();

			if (SoccerCharacter && SoccerGameMode)
			{
				SoccerGameMode->PlayerScoreUpdate(SoccerCharacter, Cast<ASoccerPlayerController>(SoccerCharacter->GetController()), Score);
			
				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(
						this,
						HitSound,
						GetActorLocation()
					);
				}

				if (HitEffectRight)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						this,
						HitEffectRight,
						HitEffectComponentRight->GetComponentLocation(),
						GetActorRotation()
					);
				}

				if (HitEffectLeft)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						this,
						HitEffectLeft,
						HitEffectComponentLeft->GetComponentLocation(),
						GetActorRotation()
					);
				}

				bHitDetected = true;
			}
	}
}

