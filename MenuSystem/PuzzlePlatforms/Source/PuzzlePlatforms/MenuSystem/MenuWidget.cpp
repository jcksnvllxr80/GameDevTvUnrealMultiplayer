// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}

void UMenuWidget::Setup()
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

void UMenuWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
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
