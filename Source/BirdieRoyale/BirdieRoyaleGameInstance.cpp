// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdieRoyaleGameInstance.h"

#include "UI/MainMenu.h"

#include "Engine/Engine.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UBirdieRoyaleGameInstance::UBirdieRoyaleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UMainMenu> MenuBPClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
	if (MenuBPClass.Class != nullptr)
	{
		MenuClass = MenuBPClass.Class;
	}
}

void UBirdieRoyaleGameInstance::Init()
{
	Super::Init();
}

void UBirdieRoyaleGameInstance::LoadMainMenu()
{
	if (MenuClass == nullptr)
	{
		return;
	}

	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
	if (MainMenu == nullptr)
	{
		return;
	}

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UBirdieRoyaleGameInstance::CloseMainMenu()
{
	if (MainMenu != nullptr)
	{
		MainMenu->TearDown();
		MainMenu = nullptr;
	}
}

void UBirdieRoyaleGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (Engine == nullptr)
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Hosting Game"));

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	World->ServerTravel("/Game/Levels/Lobby?listen");
}

void UBirdieRoyaleGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (Engine == nullptr)
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Joining Game @ " + Address));

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}