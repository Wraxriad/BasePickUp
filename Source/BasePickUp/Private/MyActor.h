// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class UStaticMeshComponent;
UCLASS()
class AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PickUp")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
};
