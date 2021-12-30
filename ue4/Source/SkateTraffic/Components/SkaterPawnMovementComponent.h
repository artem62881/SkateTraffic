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
UENUM(BlueprintType)
enum class EJumpType : uint8
{
	Failed = 0,
	Succesful
};

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

	void Jump(EJumpType JumpType);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PushForwardMaxVelocity = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCurveFloat* PushForwardVelocityCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SlowDownVelocity = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SuccesfulJumpVelocity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FailedJumpVelocity = 200.0f;
	
private:
	class ASkaterPawn* PawnOwner = nullptr;

	float CurrentPushForwardMaxVelocity = 0.f;
	float CurrentPushForwardVelocity = 0.f;
	FTimeline PushForwardVelocityTimeline;
	void PushForwardUpdate(float DeltaTime);
	void PushForwardVelocityUpdate(float Alpha);
	
	void SlowDownUpdate(float DeltaTime);
};
