// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "SkateTrafficTypes.h"
#include "Actor/STMusicActor.h"
#include "GameFramework/GameModeBase.h"
#include "SkateTrafficGameMode.generated.h"

UCLASS(minimalapi)
class ASkateTrafficGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASkateTrafficGameMode();

	virtual void StartPlay() override;
	
	FOnGameStateChangedSignature OnGameStateChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASTMusicActor> LevelMusic;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStateChangedEvent(ESTGameState NewGameState);
	
	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate /*= FCanUnpause()*/) override;
	virtual bool ClearPause() override;

	UPROPERTY(BlueprintReadWrite)
	ASTMusicActor* CurrentLevelMusicActor = nullptr;
	
private:
	ESTGameState CurrentGameState = ESTGameState::WaitingToStart;
	
	void GameOver();
	
	void SetGameState(ESTGameState NewGameState);



};



