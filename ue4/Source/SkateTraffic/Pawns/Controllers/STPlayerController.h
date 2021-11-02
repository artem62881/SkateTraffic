// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../STBasePawn.h"
#include "GameFramework/Pawn.h"
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
	virtual void SetupInputComponent() override;

private:
	void PushForward();
	void SlowDownStart();
	void SlowDownStop();
	void SwitchLaneRight();
	void SwitchLaneLeft();

	TSoftObjectPtr<class ASkaterPawn> CachedBasePawn;


};
