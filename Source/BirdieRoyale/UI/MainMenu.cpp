// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	if (!ensure(HostButton != nullptr) ||
		!ensure(JoinButton != nullptr) || 
		!ensure(JoinBackButton != nullptr) ||
		!ensure(MainMenuSwitcher != nullptr) ||
		!ensure(ConnectButton != nullptr))
	{
		return false;
	}

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	JoinBackButton->OnClicked.AddDynamic(this, &UMainMenu::GoToMainMenu);
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}

void UMainMenu::Setup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* Interface)
{
	MenuInterface = Interface;
}

void UMainMenu::TearDown()
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

void UMainMenu::HostServer()
{
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	const FString& Address = IPAddressField->GetText().ToString();
	MenuInterface->Join(Address);
}

void UMainMenu::OpenJoinMenu()
{
	MainMenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::GoToMainMenu()
{
	MainMenuSwitcher->SetActiveWidget(MainMenu);
}
