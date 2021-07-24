// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "PuzzlePlatforms/PuzzlePlatformsGameInstance.h"

bool UMainMenu::Initialize()
{
	const bool SuccessfulInit = Super::Initialize();
	if (!SuccessfulInit) return false;
	
	if (Host)
	{
		Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Host button during init."));
		return false;
	}
	
	if (Join)
	{
		Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Join button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Init complete!"));
	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Display, TEXT("Hosting a server"))
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Display, TEXT("Joining a server"))
}
