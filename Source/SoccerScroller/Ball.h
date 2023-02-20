#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class SOCCERSCROLLER_API ABall : public AActor
{
	GENERATED_BODY()

public:
	ABall();
	virtual void Tick(float DeltaTime) override;
	void DisablePhysics(bool bIsDisable);
	void SetBallPhysicsToShooting();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float StartLinearDamping = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float StartAngularDamping = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float StartMassScale = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float ShootLinearDamping = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float ShootAngularDamping = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float ShootMassScale = 500.0f;

public:
	FORCEINLINE UStaticMeshComponent* GetBallMesh() const { return BallMesh; }
};
