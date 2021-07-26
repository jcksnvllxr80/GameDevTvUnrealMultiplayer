// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenJoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* HostIpAddress;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void BackToMainMenu();

	IMenuInterface* MenuInterface;
	
};
