// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class BIRDIEROYALE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSliding() { return bIsSliding; }

private:
	void SetupActiveRagdoll();
	bool CanSlide() const;
	bool IsSlideBoostAvailable() const;
	bool IsPauseMenuOpen() const;

	// Event Handlers
	UFUNCTION()
	void OnMeshHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Move(float AxisValue);

	UFUNCTION()
	void Turn(float AxisValue);

	UFUNCTION()
	void SlideStart();

	UFUNCTION()
	void SlideStop();

	UFUNCTION()
	void SlideReset();

	UFUNCTION()
	void ZoomCameraIn();

	UFUNCTION()
	void ZoomCameraOut();

	UFUNCTION()
	void TryTogglePauseMenu();

	// Components
	class USpringArmComponent* SpringArm = nullptr;
	class UCameraComponent* Camera = nullptr;
	class UPhysicalAnimationComponent* PhysicalAnimation = nullptr;

	// Initial Values
	float DefaultMaxWalkSpeed = 0.0f;
	float DefaultGroundFriction = 0.0f;

	// Camera
	float ZoomStep = 50.0f;
	float MinSpringArmLength = 300.0f;
	float MaxSpringArmLength = 1000.0f;
	
	// Movement
	float SlideBoostCooldown = 2.0f;
	float SlideBoostStrength = 1200.0f;
	float SlideExitDelay = 1.0f;
	float SlideTurnDampening = 0.25f;
	float SlideGroundFriction = 0.5f;
	float PostSlideMaxWalkSpeed = 180.0f;
	float PushForceMultiplier = 150.0f;
	float HitForceMultiplier = 0.25f;
	float MaxHitForce = 70.0f;
	
	// Animation
	float LastSlideStartedTs = 0.0f;
	float NextSlideBoostAvailableTs = 0.0f;
	bool bIsSliding = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName RagdollBoneName;
};
