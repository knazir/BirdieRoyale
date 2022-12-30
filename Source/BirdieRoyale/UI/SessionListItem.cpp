// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionListItem.h"

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool USessionListItem::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)
	{
		return false;
	}

	if (!ensure(SelectButton != nullptr) || !ensure(SessionName != nullptr))
	{
		return false;
	}

	SelectButton->OnClicked.AddDynamic(this, &USessionListItem::SelectSession);

	return true;
}

void USessionListItem::Setup(UMainMenu* Parent, uint32 Index, const FString& Name)
{
	ParentMainMenu = Parent;
	SessionIndex = Index;
	SessionName->SetText(FText::FromString(Name));
}

void USessionListItem::SelectSession()
{
	if (ensure(ParentMainMenu != nullptr))
	{
		ParentMainMenu->SetSelectedSessionIndex(SessionIndex);
	}
}

