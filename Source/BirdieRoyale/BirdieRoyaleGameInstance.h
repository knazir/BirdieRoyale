// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "UI/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BirdieRoyaleGameInstance.generated.h"

UCLASS()
class BIRDIEROYALE_API UBirdieRoyaleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UBirdieRoyaleGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	
	void TryTogglePauseMenu();
	void ClosePauseMenu();
	bool IsPauseMenuOpen() const;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void CloseMainMenu();

	virtual void Host() override;
	virtual void Join(uint32 SessionIndex) override;
	virtual void RefreshSessionsList() override;

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	class UMainMenu* MainMenu = nullptr;

	TSubclassOf<UUserWidget> PauseMenuClass;
	class UPauseMenu* PauseMenu = nullptr;

	IOnlineSessionPtr SessionInterface = nullptr;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch = nullptr;

	void CreateNewSession();
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
