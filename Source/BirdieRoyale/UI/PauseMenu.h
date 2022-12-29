// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class BIRDIEROYALE_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void TearDown();
	
protected:
	virtual bool Initialize() override;

private:
	// Event Handlers
	UFUNCTION()
	void LeaveGame();

	UFUNCTION()
	void QuitGame();

	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	// Bound Widgets
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitToDesktopButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;
};
