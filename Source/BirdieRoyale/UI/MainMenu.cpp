// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "SessionListItem.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/PanelWidget.h"
#include "Components/Throbber.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UClass> SessionListItemBPClass(TEXT("/Game/Blueprints/UI/WBP_SessionListItem.WBP_SessionListItem_C"));
	if (ensure(SessionListItemBPClass.Object != nullptr))
	{
		SessionListItemClass = SessionListItemBPClass.Object;
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	if (!ensure(HostButton != nullptr) ||
		!ensure(JoinButton != nullptr) || 
		!ensure(QuitButton != nullptr) ||
		!ensure(JoinBackButton != nullptr) ||
		!ensure(MainMenuSwitcher != nullptr) ||
		!ensure(RefreshButton != nullptr) ||
		!ensure(ConnectButton != nullptr))
	{
		return false;
	}

	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	JoinBackButton->OnClicked.AddDynamic(this, &UMainMenu::GoToMainMenu);
	RefreshButton->OnClicked.AddDynamic(this, &UMainMenu::RefreshSessionsList);
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}

void UMainMenu::Setup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
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
	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (IsInViewport())
	{
		RemoveFromParent();
	}

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::SetSessionsList(TArray<FString> SessionNames)
{
	LoadingThrobber->SetVisibility(ESlateVisibility::Hidden);
	SessionList->ClearChildren();

	for (int32 SessionIndex = 0; SessionIndex < SessionNames.Num(); SessionIndex++)
	{
		const FString& SessionName = SessionNames[SessionIndex];
		USessionListItem* SessionListItem = CreateWidget<USessionListItem>(GetWorld(), SessionListItemClass);
		SessionListItem->Setup(this, SessionIndex, SessionName);
		SessionList->AddChild(SessionListItem);
	}
}

void UMainMenu::SetSelectedSessionIndex(uint32 Index)
{
	SelectedSessionIndex = Index;
}

void UMainMenu::HostServer()
{
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (SelectedSessionIndex.IsSet())
	{
		MenuInterface->Join(SelectedSessionIndex.GetValue());
	}
}

void UMainMenu::OpenJoinMenu()
{
	MainMenuSwitcher->SetActiveWidget(JoinMenu);
	RefreshSessionsList();
}

void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UMainMenu::RefreshSessionsList()
{
	SessionList->ClearChildren();
	LoadingThrobber->SetVisibility(ESlateVisibility::Visible);
	MenuInterface->RefreshSessionsList();
}


void UMainMenu::GoToMainMenu()
{
	MainMenuSwitcher->SetActiveWidget(MainMenu);
}
