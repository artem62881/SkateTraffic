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

	virtual void SwitchLaneStart(int32 Direction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitialVelocity = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PushForwardMaxVelocity = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveFloat* PushForwardVelocityCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SlowDownVelocity = 300.0f;

private:
	class ASkaterPawn* PawnOwner = nullptr;

	float CurrentPushForwardMaxVelocity = 0.f;
	float CurrentPushForwardVelocity = 0.f;
	FTimeline PushForwardVelocityTimeline;
	void PushForwardUpdate(float DeltaTime);
	void PushForwardVelocityUpdate(float Alpha);
	
	void SlowDownUpdate(float DeltaTime);
};
