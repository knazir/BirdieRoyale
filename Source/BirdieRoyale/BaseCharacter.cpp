// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "TimerManager.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = FindComponentByClass<USpringArmComponent>();
	Camera = FindComponentByClass<UCameraComponent>();
	PhysicalAnimation = FindComponentByClass<UPhysicalAnimationComponent>();
	
	if (!ensure(SpringArm != nullptr) || !ensure(Camera != nullptr) || !ensure(PhysicalAnimation != nullptr))
	{
		return;
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	DefaultMaxWalkSpeed = Movement->MaxWalkSpeed;
	DefaultGroundFriction = Movement->GroundFriction;

	GetMesh()->OnComponentHit.AddDynamic(this, &ABaseCharacter::OnMeshHit);
	SetupActiveRagdoll();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Speed = GetVelocity().Size();
	GetCharacterMovement()->InitialPushForceFactor = Speed * PushForceMultiplier;
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &ABaseCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Slide"), IE_Pressed, this, &ABaseCharacter::SlideStart);
	PlayerInputComponent->BindAction(TEXT("Slide"), IE_Released, this, &ABaseCharacter::SlideStop);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &ABaseCharacter::ZoomCameraIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &ABaseCharacter::ZoomCameraOut);
}

void ABaseCharacter::SetupActiveRagdoll()
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	
	FPhysicalAnimationData PhysicalAnimationData;
	PhysicalAnimationData.bIsLocalSimulation = true;

	PhysicalAnimation->SetSkeletalMeshComponent(CharacterMesh);
	PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(RagdollBoneName, PhysicalAnimationData, true);
	CharacterMesh->SetAllBodiesBelowSimulatePhysics(RagdollBoneName, true, false);
}

bool ABaseCharacter::CanSlide() const
{
	return !GetCharacterMovement()->IsFalling() && !bIsSliding;
}

bool ABaseCharacter::IsSlideBoostAvailable() const
{
	return GetWorld()->GetTimeSeconds() >= NextSlideBoostAvailableTs;
}

void ABaseCharacter::OnMeshHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const float OtherActorSpeed = OtherActor->GetVelocity().Size();
	float HitForce = FMath::Min(OtherActorSpeed * HitForceMultiplier, MaxHitForce);
	
	GetCharacterMovement()->AddImpulse(Hit.Normal * HitForce, true);
}

void ABaseCharacter::Move(float AxisValue)
{
	const FVector& Velocity = GetVelocity();
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ABaseCharacter::Turn(float AxisValue)
{
	if (bIsSliding)
	{
		AddControllerYawInput(AxisValue * SlideTurnDampening);
	}
	else
	{
		AddControllerYawInput(AxisValue);
	}
}

void ABaseCharacter::SlideStart()
{
	if (!CanSlide())
	{
		return;
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	bIsSliding = true;
	LastSlideStartedTs = GetWorld()->GetTimeSeconds();
	Movement->MaxWalkSpeed = PostSlideMaxWalkSpeed;
	Movement->GroundFriction = SlideGroundFriction;
	
	if (IsSlideBoostAvailable())
	{
		const FVector Impulse = (GetActorForwardVector() * SlideBoostStrength) + GetActorForwardVector();
		Movement->AddImpulse(Impulse, true);
		NextSlideBoostAvailableTs = LastSlideStartedTs + SlideBoostCooldown;;
	}
}

void ABaseCharacter::SlideStop()
{
	if (!bIsSliding)
	{
		return;
	}

	if (GetWorld()->GetTimeSeconds() - LastSlideStartedTs < SlideExitDelay)
	{
		FTimerHandle SlideResetTimerHandle;
		GetWorldTimerManager().SetTimer(SlideResetTimerHandle, this, &ABaseCharacter::SlideReset, SlideExitDelay, false);
	}
	else
	{
		SlideReset();
	}
}

void ABaseCharacter::SlideReset()
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	bIsSliding = false;
	Movement->MaxWalkSpeed = DefaultMaxWalkSpeed;
	Movement->GroundFriction = DefaultGroundFriction;
}

void ABaseCharacter::ZoomCameraIn()
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - ZoomStep, MinSpringArmLength, MaxSpringArmLength);
}

void ABaseCharacter::ZoomCameraOut()
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + ZoomStep, MinSpringArmLength, MaxSpringArmLength);
}
