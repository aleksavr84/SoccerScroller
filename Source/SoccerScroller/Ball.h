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

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ball Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BallMesh;
};
