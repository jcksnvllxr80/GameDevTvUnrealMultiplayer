// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"

bool UMainMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Host button during init."));
		return false;
	}
	
	if (OpenJoinMenuButton)
	{
		OpenJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Open Join Menu button during init."));
		return false;
	}

	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Join button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Main Menu Init complete!"));
	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Display, TEXT("Hosting a server"))
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher)
	{
		if (JoinMenu)
		{
			MenuSwitcher->SetActiveWidget(JoinMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OPEN JKOIN MENU FAILED! Cant find the JoinMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN JOIN MENU FAILED! Cant find the Menu Switcher OBJ."));
		return;
	}
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Display, TEXT("Joining a server"))
	if (MenuInterface)
	{
		if (HostIpAddress)
		{
			FString IpAddress = HostIpAddress->GetText().ToString();
			MenuInterface->Join(IpAddress);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JOIN SERVER FAILED! Cant find the HostIpAddress Widget."));
			return;
		}
	}
}
