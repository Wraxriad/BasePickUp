#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationRoll = false;

	TObjectPtr < UCharacterMovementComponent> MovementComponent = this->GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;

	this->CameraArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TWA SpringArmComponent"));
	this->CameraArmComponent->SetupAttachment(this->RootComponent.Get());
	this->CameraArmComponent->TargetArmLength = 400.0f;
	this->CameraArmComponent->bUsePawnControlRotation = true;

	this->ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	this->ThirdPersonCamera->SetupAttachment(this->CameraArmComponent.Get(), USpringArmComponent::SocketName);
	this->ThirdPersonCamera->bUsePawnControlRotation = false;

	this->FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	this->FirstPersonCamera->SetupAttachment(this->GetMesh(), TEXT("HeadSocket"));
	this->FirstPersonCamera->bUsePawnControlRotation = true;
	this->FirstPersonCamera->bAutoActivate = false;

	//Crouch
	MovementComponent->MaxWalkSpeedCrouched = 200.f;
	MovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;

}
