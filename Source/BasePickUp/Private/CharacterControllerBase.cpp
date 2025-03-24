#include "CharacterControllerBase.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterBase.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"

void ACharacterControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(this->InputComponent);

	if (EnhancedInputComponent)
	{
		// Move
		EnhancedInputComponent->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &ACharacterControllerBase::Move);
		// Look
		EnhancedInputComponent->BindAction(LookAction.Get(), ETriggerEvent::Triggered, this, &ACharacterControllerBase::Look);
		// Jump
		EnhancedInputComponent->BindAction(JumpAction.Get(), ETriggerEvent::Started, this, &ACharacterControllerBase::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction.Get(), ETriggerEvent::Completed, this, &ACharacterControllerBase::JumpStop);
		// Run
		EnhancedInputComponent->BindAction(RunAction.Get(), ETriggerEvent::Started, this, &ACharacterControllerBase::RunStart);
		EnhancedInputComponent->BindAction(RunAction.Get(), ETriggerEvent::Completed, this, &ACharacterControllerBase::RunStop);
		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction.Get(), ETriggerEvent::Started, this, &ACharacterControllerBase::CrouchStart);
		EnhancedInputComponent->BindAction(CrouchAction.Get(), ETriggerEvent::Completed, this, &ACharacterControllerBase::CrouchStop);
		//1st to 3rd
		EnhancedInputComponent->BindAction(PerspectiveAction.Get(), ETriggerEvent::Started, this, &ACharacterControllerBase::Perspective);
		//Dashing
		EnhancedInputComponent->BindAction(DashAction.Get(), ETriggerEvent::Started, this, &ACharacterControllerBase::Dash);
		//Interact
		EnhancedInputComponent->BindAction(InteractAction.Get(), ETriggerEvent::Started, this, &ACharacterControllerBase::Interact);
	}
}

void ACharacterControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	this->CurrentCharacter = Cast<ACharacterBase>(InPawn);

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> InputLocalPlayerSubsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());

	if (InputLocalPlayerSubsystem)
	{
		InputLocalPlayerSubsystem->AddMappingContext(this->CurrentMappingContext.Get(), 0);
	}
}

void ACharacterControllerBase::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = this->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	this->CurrentCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	this->CurrentCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void ACharacterControllerBase::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	this->CurrentCharacter->AddControllerYawInput(LookAxisVector.X);
	this->CurrentCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ACharacterControllerBase::JumpStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, TEXT("Jump"));
	this->CurrentCharacter->Jump();
}

void ACharacterControllerBase::JumpStop()
{
	this->CurrentCharacter->StopJumping();
}

void ACharacterControllerBase::RunStart()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Run"));
}

void ACharacterControllerBase::RunStop()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed /= 2;
}

void ACharacterControllerBase::CrouchStart()
{
	this->CurrentCharacter->Crouch();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Crouch"));
}

void ACharacterControllerBase::CrouchStop()
{
	this->CurrentCharacter->UnCrouch();
}

void ACharacterControllerBase::Perspective()
{
	if (FlipFlop)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("FirstP"));
		this->CurrentCharacter->ThirdPersonCamera->Deactivate();
		this->CurrentCharacter->FirstPersonCamera->Activate();
		FlipFlop = false;
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("ThirdP"));
		this->CurrentCharacter->FirstPersonCamera->Deactivate();
		this->CurrentCharacter->ThirdPersonCamera->Activate();
		FlipFlop = true;
	}
}

void ACharacterControllerBase::Dash()
{
	const FVector ForwardDirection = this->CurrentCharacter->GetActorRotation().Vector();
	if (!CurrentCharacter->GetMovementComponent()->IsFalling()) {
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.0f, FColor::Green, 
			TEXT("Ground Dash"));

		this->CurrentCharacter->LaunchCharacter(
		ForwardDirection * DashDistance,
		true,
		true
		);
	}
}

void ACharacterControllerBase::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Interact"));
}
