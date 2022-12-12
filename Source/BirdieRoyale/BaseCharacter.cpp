// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	DefaultInitialPushForceFactor = Movement->InitialPushForceFactor;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float Speed = GetVelocity().Size();
	float ActualAccelerationMag = FMath::Abs((Speed - PreviousTickSpeed) / DeltaTime);
	PreviousTickSpeed = Speed;
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	float Force = ActualAccelerationMag * GetMesh()->GetMass()* InitialPushForceMultiplier;

	Movement->InitialPushForceFactor = FMath::Min(Force, Movement->MaxTouchForce);
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

bool ABaseCharacter::IsSlideBoostAvailable() const
{
	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(this);
	return CurrentTime >= NextSlideBoostAvailableTs;
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

	// Put the character into sliding state regardless of if boost was available
	bIsSliding = true;
	LastSlideStartedTs = UGameplayStatics::GetRealTimeSeconds(this);
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = PostSlideSpeed;
	Movement->GroundFriction = SlideFriction;

	// Sliding is independent of the boost, which has a cooldown
	if (IsSlideBoostAvailable())
	{
		FVector Boost = GetActorForwardVector() * SlideBoostStrength;
		Movement->AddImpulse(GetActorForwardVector() + Boost, true);
		NextSlideBoostAvailableTs = LastSlideStartedTs + SlideBoostCooldown;
	}
}

void ABaseCharacter::StopSliding()
{
	if (!bIsSliding)
	{
		return;
	}

	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(this);
	if (CurrentTime - LastSlideStartedTs < SlideExitDelay)
	{
		FTimerHandle SlideTimerHandle;
		GetWorldTimerManager().SetTimer(SlideTimerHandle, this, &ABaseCharacter::ResetSliding, SlideExitDelay, false);
	}
	else
	{
		ResetSliding();
	}
}

void ABaseCharacter::ResetSliding()
{
	bIsSliding = false;
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = DefaultMaxWalkSpeed;
	Movement->GroundFriction = DefaultGroundFriction;
}

