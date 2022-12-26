// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "BirdieRoyaleGameInstance.generated.h"

UCLASS()
class BIRDIEROYALE_API UBirdieRoyaleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UBirdieRoyaleGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(Exec)
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join(const FString& Address) override;

private:
	TSubclassOf<UUserWidget> MenuClass;
	
	class UMainMenu* MainMenu;
};
