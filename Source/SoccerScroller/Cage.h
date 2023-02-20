#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cage.generated.h"

UCLASS()
class SOCCERSCROLLER_API ACage : public AActor
{
	GENERATED_BODY()
	
public:	
	ACage();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cage Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CageMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BallCheckCollision;

	UPROPERTY(EditAnywhere)
	class USoundCue* HitSound;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* HitEffectComponentLeft;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitEffectLeft;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* HitEffectComponentRight;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitEffectRight;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float Score = 1.f;

	bool bHitDetected = false;

	// Callbacks
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
