#include "Ball.h"

ABall::ABall()
{
 	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	SetRootComponent(BallMesh);
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetMassScale(FName(""), StartMassScale);
	BallMesh->SetLinearDamping(StartLinearDamping);
	BallMesh->SetAngularDamping(StartAngularDamping);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::SetBallPhysicsToShooting()
{
	BallMesh->SetMassScale(FName(""), ShootMassScale);
	BallMesh->SetLinearDamping(ShootLinearDamping);
	BallMesh->SetAngularDamping(ShootAngularDamping);
}

