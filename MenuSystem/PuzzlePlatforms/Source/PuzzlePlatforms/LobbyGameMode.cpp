// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	++NumberOfPlayers;
	UE_LOG(LogTemp, Warning, TEXT("There are %i players in the lobby."), NumberOfPlayers);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	--NumberOfPlayers;
}
