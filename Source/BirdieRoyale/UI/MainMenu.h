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
	IMenuInterface* MenuInterface;

	// Event Handlers
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void GoToMainMenu();

	UFUNCTION()
	void OpenJoinMenu();

	// Bound Widgets
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MainMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableText* IPAddressField;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectButton;
};
