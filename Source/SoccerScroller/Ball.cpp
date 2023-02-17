#include "Ball.h"

ABall::ABall()
{
 	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	SetRootComponent(BallMesh);
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetMassScale(FName(""), 15.0f);
	BallMesh->SetLinearDamping(5.0f);
	BallMesh->SetAngularDamping(5.0f);

	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	//CollisionBox->SetupAttachment(BallMesh);
	//CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	//CollisionBox->CanCharacterStepUp(false);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

