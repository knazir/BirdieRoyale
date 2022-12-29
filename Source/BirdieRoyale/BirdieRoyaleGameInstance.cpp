// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdieRoyaleGameInstance.h"

#include "UI/MainMenu.h"
#include "UI/PauseMenu.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UBirdieRoyaleGameInstance::UBirdieRoyaleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UMainMenu> MainMenuBPClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
	if (ensure(MainMenuBPClass.Class != nullptr))
	{
		MainMenuClass = MainMenuBPClass.Class;
	}

	ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenuBPClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu"));
	if (ensure(PauseMenuBPClass.Class != nullptr))
	{
		PauseMenuClass = PauseMenuBPClass.Class;
	}
}

void UBirdieRoyaleGameInstance::Init()
{
	Super::Init();
}

void UBirdieRoyaleGameInstance::LoadMainMenu()
{
	MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
	if (MainMenu == nullptr)
	{
		return;
	}

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UBirdieRoyaleGameInstance::TryTogglePauseMenu()
{
	if (PauseMenu == nullptr)
	{
		if (UGameplayStatics::GetCurrentLevelName(this) != TEXT("MainMenu"))
		{
			PauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuClass);
			PauseMenu->Setup();
		}
	}
	else
	{
		ClosePauseMenu();
	}
}

void UBirdieRoyaleGameInstance::CloseMainMenu()
{
	if (MainMenu != nullptr)
	{
		MainMenu->TearDown();
		MainMenu = nullptr;
	}
}

void UBirdieRoyaleGameInstance::ClosePauseMenu()
{
	if (PauseMenu != nullptr)
	{
		PauseMenu->TearDown();
		PauseMenu = nullptr;
	}
}

bool UBirdieRoyaleGameInstance::IsPauseMenuOpen() const
{
	return PauseMenu != nullptr && PauseMenu->IsInViewport();
}

void UBirdieRoyaleGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Hosting Game"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
	{
		return;
	}

	CloseMainMenu();
	World->ServerTravel("/Game/Levels/Lobby?listen");
}

void UBirdieRoyaleGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Joining Game @ " + Address));

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
	
	CloseMainMenu();
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}