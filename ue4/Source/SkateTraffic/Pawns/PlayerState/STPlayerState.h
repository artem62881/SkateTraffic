// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STPlayerState.generated.h"

/**
 * 
 */


UCLASS()
class SKATETRAFFIC_API ASTPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:	
	virtual void BeginPlay() override;
		
	void SetCurrentDistance(uint32 NewDistance);
	void AddScore(uint32 ScoreToAdd);
	
	uint32 GetCurrentDistance() const { return CurrentDistance; };
	uint32 GetCurrentScore() const { return CurrentScore; };

private:
	uint32 CurrentDistance = 0;
	uint32 CurrentScore = 0;
};
