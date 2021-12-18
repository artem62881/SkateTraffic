// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkateTraffic/SkateTrafficTypes.h"
#include "STPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:
	void PushForward();
	void SlowDownStart();
	void SlowDownStop();
	void SwitchLaneRight();
	void SwitchLaneLeft();
	void OnPauseGame();

	TSoftObjectPtr<class ASkaterPawn> CachedBasePawn;

	void OnGameStateChanged(ESTGameState State);

};
