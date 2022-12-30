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
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void Setup();
	void SetMenuInterface(IMenuInterface* Interface);
	void TearDown();
	void SetSessionsList(TArray<FString> SessionNames);
	void SetSelectedSessionIndex(uint32 Index);
	
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

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void RefreshSessionsList();

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
	class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* SessionList;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectButton;

	UPROPERTY(meta = (BindWidget))
	class UThrobber* LoadingThrobber;
	
	// Internals
	TSubclassOf<UUserWidget> SessionListItemClass;
	TOptional<uint32> SelectedSessionIndex;
};
