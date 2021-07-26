// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

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
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Back button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Init complete!"));
	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(this->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find World obj."));
		return;
	}
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	this->RemoveFromViewport();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = false;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find World obj."));
		return;
	}
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
		FString IpAddress = TEXT("127.0.0.1");
		MenuInterface->Join(IpAddress);
	}
}

void UMainMenu::BackToMainMenu()
{
	if (MenuSwitcher)
	{
		if (MainMenu)
		{
			MenuSwitcher->SetActiveWidget(MainMenu);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OPEN JKOIN MENU FAILED! Cant find the MainMenu Widget."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OPEN MAIN MENU FAILED! Cant find the Menu Switcher OBJ."));
		return;
	}
}
