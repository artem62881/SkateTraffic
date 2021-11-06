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

	void SetInitialValues();
	float GetCurrentTargetSpeed() const;
	void SetCurrentTargetSpeed(float NewTargetSpeed);
	float GetBaseTargetSpeed() const;
	
	void IncreaseSpeed(float DeltaTime);
	void DecreaseSpeed(float DeltaTime);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Initial values | Speed")
	float MinBaseTargetSpeed = 1300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Initial values | Speed")
	float MaxBaseTargetSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Initial values | Accel")
	float MinBaseAccel = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Initial values | Accel")
	float MaxBaseAccel = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Initial values | Brake")
	float MinBaseBrakeAccel = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Initial values | Brake")
	float MaxBaseBrakeAccel = 700.f;
	
private:
	class ACarPawn* PawnOwner = nullptr;

	float BaseTargetSpeed = 0.f;
	float CurrentTargetSpeed = 0.f;
	float BaseAccel = 0.f;
	float BaseBrakeAccel = 0.f;
};