// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "ServerRow.h"
#include "Components/TextBlock.h"

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

	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("INIT FAILED! Cant find the Desktop button during init."));
		return false;
	}

	UE_LOG(LogTemp, Display, TEXT("Main Menu Init complete!"));
	return true;
}

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ServerRowBPClass.Class) return;
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UE_LOG(LogTemp, Display, TEXT("Adding available servers to server list."))
	if (ServerRowClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Main Menu blueprint class %s."), *ServerRowClass->GetName());
		UWorld* World = this->GetWorld();
		if (World)
		{
			ServerList->ClearChildren();
			for (const FString& ServerName : ServerNames)
			{
				UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
				if (ServerRow)
				{
					if (ServerList)
					{
						ServerRow->ServerListItem->SetText(FText::FromString(ServerName));
						ServerList->AddChild(ServerRow);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("ServerList object is null."));
						return;
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Cant create widget ServerRow."));
					return;
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant get World from this object."));
			return;
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find the Main Menu blueprint class."));
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
			if (MenuInterface)
			{
				MenuInterface->RefreshServerList();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("No Menu interface!"));
				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("OPEN JOIN MENU FAILED! Cant find the JoinMenu Widget."));
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
	if (MenuInterface)
	{
		// if (HostIpAddress)
		if (true) // TODO: fix this always true
		{
			// FString IpAddress = HostIpAddress->GetText().ToString();
			// MenuInterface->Join(IpAddress);
			FString Temp = TEXT("asdf");
			MenuInterface->Join(Temp);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JOIN SERVER FAILED! Cant find the HostIpAddress Widget."));
			return;
		}
	}
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			const FString QuitCommand = TEXT("quit");
			PlayerController->ConsoleCommand(QuitCommand);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cant find player controller."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find World."));
		return;
	}

}
