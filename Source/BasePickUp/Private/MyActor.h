#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class UStaticMeshComponent;
class URotatingMovementComponent;
class USphereComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	UFUNCTION()
	void OnBeginOverLapComponentEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<URotatingMovementComponent> RotatingMovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<USphereComponent> ColliderComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<UNiagaraSystem> OnPickupEffect;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	float VolumeMultiplier = {0.5};

	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	float PickEffectSpawnOffset = {90};
};
