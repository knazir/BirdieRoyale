// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdieRoyaleGameInstance.h"

#include "UI/MainMenu.h"
#include "UI/PauseMenu.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

const static FName SESSION_NAME = TEXT("My Session");

UBirdieRoyaleGameInstance::UBirdieRoyaleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UClass> MainMenuBPClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu.WBP_MainMenu_C"));
	if (ensure(MainMenuBPClass.Object != nullptr))
	{
		MainMenuClass = MainMenuBPClass.Object;
	}

	ConstructorHelpers::FObjectFinder<UClass> PauseMenuBPClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (ensure(PauseMenuBPClass.Object != nullptr))
	{
		PauseMenuClass = PauseMenuBPClass.Object;
	}
}

void UBirdieRoyaleGameInstance::Init()
{
	Super::Init();

	
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem == nullptr)
	{
		return;
	}

	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBirdieRoyaleGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBirdieRoyaleGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBirdieRoyaleGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBirdieRoyaleGameInstance::OnJoinSessionComplete);
	}
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
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr)
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else
	{
		CreateNewSession();
	}
}

void UBirdieRoyaleGameInstance::CreateNewSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 6;
	SessionSettings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UBirdieRoyaleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		GetEngine()->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Could not create session"));
		return;
	}

	GetEngine()->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Created new session"));
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

void UBirdieRoyaleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		GetEngine()->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Could not destroy existing session"));
	}
	else
	{
		CreateNewSession();
	}
}

void UBirdieRoyaleGameInstance::OnFindSessionsComplete(bool Success)
{
	FOnlineSessionSearch* SearchResults = SessionSearch.Get();
	if (SearchResults == nullptr || MainMenu == nullptr)
	{
		return;
	}

	TArray<FString> SessionNames;
	for (const FOnlineSessionSearchResult& Result : SearchResults->SearchResults)
	{
		SessionNames.Push(Result.GetSessionIdStr());
	}
	MainMenu->SetSessionsList(SessionNames);
}

void UBirdieRoyaleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		GetEngine()->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, TEXT("Could not get connection string for session"));
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))
	{
		return;
	}

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UBirdieRoyaleGameInstance::Join(uint32 SessionIndex)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		return;
	}

	CloseMainMenu();
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[SessionIndex]);
}

void UBirdieRoyaleGameInstance::RefreshSessionsList()
{
	if (SessionSearch == nullptr)
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
	}

	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}