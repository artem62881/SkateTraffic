// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SkateTraffic/Pawns/PursuerPawn.h"
#include "PursuerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API UPursuerMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float InitialVelocity = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AdditionalVelocity = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IncreaseVelocityTime = 5.f;

private:
	TWeakObjectPtr<APursuerPawn> CachedOwner;

	float CurrentVelocity = 0.f;

	FTimerHandle IncreaseVelocityTimer;
	void IncreaseVelocity();
	
};