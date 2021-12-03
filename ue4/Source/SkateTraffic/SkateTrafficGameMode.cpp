// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateTrafficGameMode.h"
#include "UI/STGameHUD.h"
#include "Pawns/PlayerPawn.h"
#include "Pawns/Controllers/STPlayerController.h"
#include "Pawns/PlayerState/STPlayerState.h"

ASkateTrafficGameMode::ASkateTrafficGameMode()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = ASTGameHUD::StaticClass();
	PlayerStateClass = ASTPlayerState::StaticClass();
	PlayerControllerClass = ASTPlayerController::StaticClass();
}

void ASkateTrafficGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetGameState(ESTGameState::InProgress);
}



void ASkateTrafficGameMode::GameOver()
{
	SetGameState(ESTGameState::GameOver);
}

void ASkateTrafficGameMode::SetGameState(ESTGameState NewGameState)
{
	if (CurrentGameState == NewGameState)
	{
		return;
	}
	CurrentGameState = NewGameState;
	OnGameStateChanged.Broadcast(CurrentGameState);
}
