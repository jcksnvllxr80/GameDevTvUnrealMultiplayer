// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include <ThirdParty/CryptoPP/5.6.5/include/argnames.h>

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PlatformTrigger.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!MainMenuBPClass.Class) return;
	MenuClass = MainMenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!InGameMenuBPClass.Class) return;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find IOnlineSubsystem, exiting Init early."));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Hello, got the IOnlineSubsystem, %s, from UPuzzlePlatformsGameInstance Init."),
		*Subsystem->GetSubsystemName().ToString());
	SessionInterface = Subsystem->GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find SessionInterface, exiting Init early."));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Found the SessionInterface."));
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
		this, &UPuzzlePlatformsGameInstance::OnGameSessionComplete);
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
		this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
	this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);

	GameSessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (!GameSessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GameSessionSearch, is not valid! Cancel find sessions."));
		return;
	}
	// GameSessionSearch->bIsLanQuery = true;
	// GameSessionSearch->QuerySettings.Set(key, value);  // will use in the future with steam
	UE_LOG(LogTemp, Display, TEXT("Starting session search."));
	SessionInterface->FindSessions(0, GameSessionSearch.ToSharedRef());
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("There is no SessionInterface, exiting Host func early."));
		return;
	}
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (!ExistingSession)
	{
		CreateSession();
		return;
	}
	SessionInterface->DestroySession(SESSION_NAME);
}

void UPuzzlePlatformsGameInstance::Join(FString& IpAddress)
{
	UEngine* Engine = GetEngine();
	if (!Engine) return;
	Engine->AddOnScreenDebugMessage(0,5,FColor::Green,
		FString::Printf(TEXT("Joining %s"), *IpAddress));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;
	PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (MenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu blueprint class %s."), *MenuClass->GetName());
		Menu = CreateWidget<UMainMenu>(this, MenuClass);
		if (Menu)
		{
			Menu->Setup();
			Menu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMainMenu Menu from main menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
		return;
	}
	
}

void UPuzzlePlatformsGameInstance::InGameLoadMenu()
{
	if (InGameMenuClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found In Game Menu blueprint class %s."), *InGameMenuClass->GetName());
		UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
		if (InGameMenu)
		{
			InGameMenu->Setup();
			InGameMenu->SetMenuInterface(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant create UMenuWidget Menu from main menu blueprint class."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
		return;
	}
}

void UPuzzlePlatformsGameInstance::OnGameSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Not able to create a session."));
		return;
	}
	UEngine* Engine = GetEngine();
	if (!Engine) return;
	Engine->AddOnScreenDebugMessage(0,2,FColor::Green,TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!World) return;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Display, TEXT("Session, %s, Destroyed successfully."));
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogTemp, Display, TEXT("Finding sessions is complete."));
	if (Success)
	{
		if (!GameSessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("GameSessionSearch is not valid. Cant get find session results."));
			return;
		}
		TArray<FOnlineSessionSearchResult> SessionSearchResults = GameSessionSearch->SearchResults;
		if (SessionSearchResults.Num() > 0)
		{
			for (const FOnlineSessionSearchResult& SessionSearchResult : SessionSearchResults)
			{
				UE_LOG(LogTemp, Display, TEXT("Found session, %s with ping: %i ms."),
					*SessionSearchResult.GetSessionIdStr(), SessionSearchResult.PingInMs);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("There were 0 game sessions found in the find session search."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game session search was not successful."));
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Creating session, %s."), *SESSION_NAME.ToString());
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}
