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
	bool IsSlideBoostAvailable() const;
	
	UFUNCTION(BlueprintPure)
	bool IsSliding() const;

private:
	void MoveForward(float AxisValue);
	void TurnRight(float AxisValue);
	void Jump();
	void StartSliding();
	void StopSliding();
	void ResetSliding();

	bool bIsSliding = false;
	float DefaultMaxWalkSpeed = 0.0f;
	float DefaultGroundFriction = 0.0f;
	float LastSlideStartedTs = 0.0f;
	float NextSlideBoostAvailableTs = 0.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideBoostCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideBoostStrength = 1200.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideTurnDampener = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideFriction = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideExitDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float PostSlideSpeed = 180.0;
};
