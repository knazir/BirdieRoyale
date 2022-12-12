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


private:
	// Blueprint Only
	
	UPROPERTY(BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsSliding = false;

	UPROPERTY(BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	float NextSlideBoostAvailableTs = 0.0f;

	UPROPERTY(BluePrintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LastSlideStartedTs = 0.0f;

	// Configurable
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideBoostCooldown = 2.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideBoostStrength = 1200.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float VelocityInitialPushForceMultiplier = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideExitDelay = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideTurnDampener = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SlideFriction = 0.5f;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float PostSlideSpeed = 180.0;
};
