#include "GameFramework/RotatingMovementComponent.h"
#include "MyActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ColliderComponent = CreateDefaultSubobject<USphereComponent>("ColliderComponent");
	SetRootComponent(ColliderComponent);
	ColliderComponent->SetGenerateOverlapEvents(true);
	ColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::OnBeginOverLapComponentEvent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(ColliderComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetGenerateOverlapEvents(false);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingMovementComponent");
}

void AMyActor::OnBeginOverLapComponentEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{//Change Cast
	if (!Cast<ACharacter>(OtherActor)) return;

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), VolumeMultiplier);
	}

	if (OnPickupEffect)
	{
		const FVector Offset = GetActorUpVector() * PickEffectSpawnOffset;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, OnPickupEffect, OtherActor->GetActorLocation() + Offset);
	}
	Destroy();
}