// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);
	void SetServerList(TArray<FString> ServerNames);
	TOptional<uint32> GetSelectedIndex() const;
	void SetSelectedIndex(const TOptional<uint32>& SelectedIndex);

private:
	TSubclassOf<class UUserWidget> ServerRowClass = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenJoinMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DesktopButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* HostIpAddress;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void BackToMainMenu();

	TOptional<uint32> SelectedIndex;
};
