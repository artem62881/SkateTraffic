// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "SkateTrafficTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SkateTrafficGameMode.generated.h"

UCLASS(minimalapi)
class ASkateTrafficGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASkateTrafficGameMode();

	virtual void BeginPlay() override;
	
	FOnGameStateChangedSignature OnGameStateChanged;
	
private:
	ESTGameState CurrentGameState = ESTGameState::WaitingToStart;
	
	void GameOver();
	
	void SetGameState(ESTGameState NewGameState);
};



