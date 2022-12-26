// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BIRDIEROYALE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void SetMenuInterface(IMenuInterface* Interface);
	void TearDown();
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	IMenuInterface* MenuInterface;
};
