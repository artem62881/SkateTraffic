// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawnMovementComponent.h"
#include "CarPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API UCarPawnMovementComponent : public USTBasePawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseVelocity = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseAccel = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseBrakeAccel = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FrontCarCheckLineTraceLength = 1000.f;

	UFUNCTION(BlueprintCallable)
	void IncreaseSpeed(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void DecreaseSpeed(float DeltaTime);
	
private:
	class ACarPawn* PawnOwner = nullptr;
	
	void UpdateCurrentSpeed(float DeltaTime);
};
