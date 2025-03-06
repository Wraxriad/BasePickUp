#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPT_CoinPickUpActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class BASEPICKUP_API ACPT_CoinPickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPT_CoinPickUpActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
};
