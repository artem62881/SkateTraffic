// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawnMovementComponent.h"
#include <Components/TimelineComponent.h>
#include <Curves/CurveFloat.h>
#include "SkaterPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API USkaterPawnMovementComponent : public USTBasePawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PushForwardStart();
	void PushForwardEnd();

	virtual void SwitchLaneStart(int8 Direction) override;

	//float GetLaneXLocationPerNum(int8 Num) const;
	//int8 GetCurrentLaneNum() const;

	//bool IsSwitchingLanes() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PushForwardMaxVelocity = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveFloat* PushForwardVelocityCurve;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//float SwitchLaneSpeed = 600.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//float SwitchLaneMinVelocity = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SlowDownVelocity = 300.0f;
	
private:
	class ASkaterPawn* PawnOwner = nullptr;

	float CurrentPushForwardMaxVelocity = 0.f;
	float CurrentPushForwardVelocity = 0.f;
	FTimeline PushForwardVelocityTimeline;
	void PushForwardUpdate(float DeltaTime);
	void PushForwardVelocityUpdate(float Alpha);

	//void SwitchLaneUpdate(float DeltaTime);
	//void SwitchLaneEnd();

	void SlowDownUpdate(float DeltaTime);

	//bool bIsSwitchingLanes = false;
	//FVector CachedPawnInitialLocation = FVector::ZeroVector;
	//FVector CurrentSwitchLaneVector = FVector::ZeroVector;
	//FVector CurrentSwitchLaneInitialLocation = FVector::ZeroVector;

	

};
