// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SkateTraffic/Pawns/CarPawn.h"
#include "CarAIController.generated.h"

/**
 * 
 */
class UBehaviorTree;
UCLASS()
class SKATETRAFFIC_API ACarAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;;
	virtual void Tick(float DeltaSeconds) override;

	
private:
	TWeakObjectPtr<class ACarPawn> CachedPawnOwner;

	void UpdateCurrentSpeed(float DeltaTime);
};
