// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	

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
	if (!ensure(World != nullptr))
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))
	{
		return;
	}

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
	if (!ensure(World != nullptr))
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))
	{
		return;
	}

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
	if (!ensure(MenuInterface != nullptr))
	{
		return;
	}
	
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (!ensure(MenuInterface != nullptr) || !ensure(IPAddressField != nullptr))
	{
		return;
	}
	const FString& Address = IPAddressField->GetText().ToString();
	
	if (!ensure(MenuInterface != nullptr))
	{
		return;
	}
	MenuInterface->Join(Address);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MainMenuSwitcher != nullptr) || !ensure(JoinMenu != nullptr))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Open Join Menu"));

	MainMenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::GoToMainMenu()
{
	if (!ensure(MainMenuSwitcher != nullptr) || !ensure(MainMenu != nullptr))
	{
		return;
	}

	MainMenuSwitcher->SetActiveWidget(MainMenu);
}
