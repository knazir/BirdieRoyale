// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdieRoyaleGameInstance.h"

void UBirdieRoyaleGameInstance::Init()
{
	
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

	World->ServerTravel("/Game/Levels/Test?listen");
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