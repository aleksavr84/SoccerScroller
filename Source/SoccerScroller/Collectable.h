#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable.generated.h"

UCLASS()
class SOCCERSCROLLER_API ACollectable : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollectable();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* PickupEffectComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float Score = 1.f;

	// Callbacks
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:	
	virtual void Tick(float DeltaTime) override;

};
