// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawnMovementComponent.h"
#include "CarPawnMovementComponent.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCarSpeedValues 
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinTargetSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxTargetSpeed = 1300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinAccel = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAccel = 700.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float MinBrakeAccel = 400.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float MaxBrakeAccel = 700.f;
};

UCLASS()
class SKATETRAFFIC_API UCarPawnMovementComponent : public USTBasePawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetCarSpeedValues(FCarSpeedValues SpeedValues);
	
	float GetCurrentTargetSpeed() const;
	
	void SetCurrentTargetSpeed(float NewTargetSpeed);
	
	void IncreaseSpeed(float DeltaTime);
	void DecreaseSpeed(float DeltaTime);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCarSpeedValues InitialSpeedValues;
	
private:
	class ACarPawn* PawnOwner = nullptr;

	//float BaseTargetSpeed = 0.f;
	UPROPERTY(VisibleAnywhere)
	float CurrentTargetSpeed = 0.f;

	UPROPERTY(VisibleAnywhere)
	float BaseAccel = 0.f;

	//UPROPERTY(VisibleAnywhere)
	//float BaseBrakeAccel = 0.f;
	uint32 InitialLane = 1;
};