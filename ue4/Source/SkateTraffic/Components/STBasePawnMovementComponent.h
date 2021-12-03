// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "../Pawns/STBasePawn.h"
#include "STBasePawnMovementComponent.generated.h"

/**
 * 
 */
 DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGroundHit, FVector&, HitNormal);

UCLASS()
class SKATETRAFFIC_API USTBasePawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddVelocity(FVector AdditionalVelocity_In);

	FVector GetCurrentVelocity() const;

	virtual bool IsFalling() const override;

	bool IsStanding() const;
	void SetIsStanding(bool bIn);

	virtual void SwitchLaneStart(int32 Direction);

	TArray<float> GetLanesLocationsArray() const;
	float GetLaneXLocationPerNum(int8 Num) const;
	int8 GetCurrentLaneNum() const;
	void SetCurrentLaneNum(int32 NewNum);
	
	bool IsSwitchingLanes() const;

	FOnGroundHit OnGroundHit;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gravity");
	bool bEnableGravity = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gravity");
	float FloorCheckTraceLength = 120.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRotateTowardsGradient = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float MaxVelocity = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> LanesXLocations = { -450.f, 0.f, 450.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinSwitchLaneSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSwitchLaneSpeed = 800.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (UIMin = 0.f, ClampMin = 0.f, UIMax = 1.f, ClampMax = 1.f))
	float SwitchLaneVelocityRatio = 0.4f;

	virtual void SetPawnInitialVelocity(FVector InVelocity);

private:
	void SetPawnVelocity(FVector NewVelocity);
	
	void UpdatePawnAcceleration(float DeltaTime, FVector FloorNormal);

	void RotatePawnTowardsFloorGradient(FVector FloorNormal);

	void UpdateCurrentVelocity(float DeltaTime);

	bool CheckFloor(float DeltaTime, FVector& HitNormal);

	UFUNCTION()
	void OnGroundHitEvent(FVector& HitNormal);

	FVector CurrentPawnAcceleration = FVector::ZeroVector;

	bool bIsFalling = false;
	bool bOnGroundHitCheck = false;	

	float SurfaceFriction = 1.0f;

	bool bIsStanding = false;

	void SwitchLaneUpdate(float DeltaTime);
	void SwitchLaneEnd();

	int8 CurrentLaneNum = 1;

	bool bIsSwitchingLanes = false;
	float CurrentSwitchLaneVelocity = 0.f;
	FVector CachedPawnInitialLocation = FVector::ZeroVector;
	FVector CurrentSwitchLaneVector = FVector::ZeroVector;
	float CurrentSwitchLaneInitialLocationX = 0.f;
};
