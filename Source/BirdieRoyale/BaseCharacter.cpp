// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Save initial settings to restore after actions
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	DefaultMaxWalkSpeed = Movement->MaxWalkSpeed;
	DefaultGroundFriction = Movement->GroundFriction;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &ABaseCharacter::TurnRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Slide"), EInputEvent::IE_Pressed, this, &ABaseCharacter::StartSliding);
	PlayerInputComponent->BindAction(TEXT("Slide"), EInputEvent::IE_Released, this, &ABaseCharacter::StopSliding);
}

bool ABaseCharacter::IsSliding() const
{
	return bIsSliding;
}

void ABaseCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ABaseCharacter::TurnRight(float AxisValue)
{
	float TurnValue = AxisValue;
	if (bIsSliding)
	{
		TurnValue *= SlideTurnDampener;
	}
	AddControllerYawInput(TurnValue);
}

void ABaseCharacter::Jump()
{
	ACharacter::Jump();
}

void ABaseCharacter::StartSliding()
{
	if (bIsSliding)
	{
		return;
	}

	bIsSliding = true;
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = PostSlideSpeed;
	Movement->GroundFriction = SlideFriction;

	FVector Boost = GetActorForwardVector() * SlideBoostStrength;
	Movement->AddImpulse(GetActorForwardVector() + Boost, true);
}

void ABaseCharacter::StopSliding()
{
	if (!bIsSliding)
	{
		return;
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::ResetSliding, SlideExitDelay, false);
}

void ABaseCharacter::ResetSliding()
{
	bIsSliding = false;
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = DefaultMaxWalkSpeed;
	Movement->GroundFriction = DefaultGroundFriction;
}

