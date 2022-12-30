// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionListItem.generated.h"

/**
 * 
 */
UCLASS()
class BIRDIEROYALE_API USessionListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup(class UMainMenu* Parent, uint32 Index, const FString& Name);

protected:
	virtual bool Initialize() override;

private:
	// Event Handlers
	UFUNCTION()
	void SelectSession();

	// Bound Widgets
	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionName;
	
	// Internals
	uint32 SessionIndex = 0;
	class UMainMenu* ParentMainMenu = nullptr;
};
