// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateTrafficGameMode.h"
#include "UI/STGameHUD.h"
#include "Pawns/PlayerPawn.h"
#include "Pawns/Controllers/STPlayerController.h"
#include "Pawns/PlayerState/STPlayerState.h"

ASkateTrafficGameMode::ASkateTrafficGameMode()
{
	FGameModeEvents::GameModeInitializedEvent.AddUObject(this, &ASkateTrafficGameMode::OnGameModeInitialized);
	
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = ASTGameHUD::StaticClass();
	PlayerStateClass = ASTPlayerState::StaticClass();
	PlayerControllerClass = ASTPlayerController::StaticClass();
}

void ASkateTrafficGameMode::StartPlay()
{
	Super::StartPlay();
	//auto CachedPlayer = Cast<APlayerPawn>(DefaultPawnClass);
	//if (IsValid(CachedPlayer))
	//{
	//	CachedPlayer->OnDeath.AddUObject(this, &ASkateTrafficGameMode::GameOver);
	//}
	OnGameStateChanged.AddUObject(this, &ASkateTrafficGameMode::OnGameStateChangedEvent);
	
	CurrentLevelMusicActor = GetWorld()->SpawnActor<ASTMusicActor>(LevelMusic, FTransform::Identity);
	SetGameState(ESTGameState::InProgress);
}

bool ASkateTrafficGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		SetGameState(ESTGameState::Pause);
	}
	
	return PauseSet;
}

bool ASkateTrafficGameMode::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();
	if (PauseCleared)
	{
		SetGameState(ESTGameState::InProgress);
	}
	
	return PauseCleared;
}

void ASkateTrafficGameMode::OnGameModeInitialized_Implementation(AGameModeBase* GameMode)
{
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
