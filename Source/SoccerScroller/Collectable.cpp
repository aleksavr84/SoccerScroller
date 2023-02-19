#include "Collectable.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "SoccerScrollerGameMode.h"
#include "SoccerPlayerController.h"
#include "SoccerScrollerCharacter.h"

ACollectable::ACollectable()
{
 		PrimaryActorTick.bCanEverTick = true;

		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

		OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
		OverlapSphere->SetupAttachment(RootComponent);
		OverlapSphere->SetSphereRadius(70.f);
		OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		OverlapSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		OverlapSphere->AddLocalOffset(FVector(0.f, 0.f, 85.f));

		PickupEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PickupEffectComponent"));
		PickupEffectComponent->SetupAttachment(RootComponent);
}

void ACollectable::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ACollectable::ActorBeginOverlap);
}

void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectable::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ASoccerScrollerCharacter* SoccerCharacter = Cast<ASoccerScrollerCharacter>(OtherActor);

	if (SoccerCharacter)
	{
		ASoccerScrollerGameMode* SoccerGameMode = GetWorld()->GetAuthGameMode<ASoccerScrollerGameMode>();
		
		if (SoccerGameMode)
		{
			SoccerGameMode->PlayerScoreUpdate(SoccerCharacter, Cast<ASoccerPlayerController>(SoccerCharacter->GetController()));
		}
	
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}

		if (PickupEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				PickupEffect,
				GetActorLocation(),
				GetActorRotation()
			);
		}

		Destroy();
	}
}


