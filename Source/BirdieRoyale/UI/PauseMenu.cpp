// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

#include "../BirdieRoyaleGameInstance.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

bool UPauseMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	if (!ensure(MainMenuButton != nullptr) || !ensure(QuitToDesktopButton != nullptr) || !ensure(CancelButton != nullptr))
	{
		return false;
	}

	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::LeaveGame);
	QuitToDesktopButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitGame);
	CancelButton->OnClicked.AddDynamic(this, &UPauseMenu::CloseMenu);

	return true;
}

void UPauseMenu::Setup()
{
	AddToViewport();
	
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UPauseMenu::TearDown()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (IsInViewport())
	{
		RemoveFromViewport();
	}

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UPauseMenu::LeaveGame()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->ClientTravel(TEXT("/Game/Levels/MainMenu"), ETravelType::TRAVEL_Absolute);
	CloseMenu();
}

void UPauseMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UPauseMenu::CloseMenu()
{
	// TODO: Figure out how to get this to trigger when you re-press the pause key
	UBirdieRoyaleGameInstance* GameInstance = Cast<UBirdieRoyaleGameInstance>(GetGameInstance());
	GameInstance->ClosePauseMenu();
}
