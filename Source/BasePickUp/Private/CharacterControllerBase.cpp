#include "CharacterControllerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterBase.h"

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
	const FVector2D MovementVector = Value.Get<FVector2d>();

	const FRotator Rotation = this->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector FowarDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	this->CurrentCharacter->AddMovementInput(FowarDirection, MovementVector.Y);
	this->CurrentCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void ACharacterControllerBase::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisVector = value.Get<FVector2d>();

	this->CurrentCharacter->AddControllerYawInput(LookAxisVector.X);
	this->CurrentCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ACharacterControllerBase::JumpStart()
{
	this->CurrentCharacter->Jump();
}

void ACharacterControllerBase::JumpStop()
{
	this->CurrentCharacter->StopJumping();
}

void ACharacterControllerBase::RunStart()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2;
}

void ACharacterControllerBase::RunStop()
{
	this->CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed /= 2;
}
