// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawnMovementComponent.h"
#include "../Pawns/CarPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTCarMovement, All, All)

void UCarPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = StaticCast<ACarPawn*>(GetOwner());
	SetCarSpeedValues(InitialSpeedValues);
}

void UCarPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCarPawnMovementComponent::SetCarSpeedValues(FCarSpeedValues SpeedValues)
{
	float BaseTargetSpeed = FMath::RandRange(SpeedValues.MinTargetSpeed, SpeedValues.MaxTargetSpeed);
	BaseAccel = FMath::RandRange(SpeedValues.MinAccel, SpeedValues.MaxAccel);
	//BaseBrakeAccel = FMath::RandRange(CarOptions.MinBrakeAccel, CarOptions.MaxBrakeAccel);

	SetCurrentTargetSpeed(BaseTargetSpeed);
}

float UCarPawnMovementComponent::GetCurrentTargetSpeed() const
{
	//UE_LOG(LogCarMovement, Display, TEXT("%s | Accel: %f"), *GetOwner()->GetName(), BaseAccel);
	return CurrentTargetSpeed;
}

void UCarPawnMovementComponent::SetCurrentTargetSpeed(float NewTargetSpeed)
{
	//UE_LOG(LogSTCarMovement, Display, TEXT("%s | TargetSpeed: %f"), *GetOwner()->GetName(), NewTargetSpeed);
	CurrentTargetSpeed = NewTargetSpeed;
}

/*float UCarPawnMovementComponent::GetBaseTargetSpeed() const
{
	return BaseTargetSpeed;
}*/

void UCarPawnMovementComponent::IncreaseSpeed(float DeltaTime)
{
	AddVelocity(PawnOwner->GetActorForwardVector() * BaseAccel * DeltaTime);
}

void UCarPawnMovementComponent::DecreaseSpeed(float DeltaTime)
{
	if (FVector::DotProduct(PawnOwner->GetActorForwardVector(), Velocity) > 0.f)
	{
		AddVelocity(-PawnOwner->GetActorForwardVector() * BaseAccel * DeltaTime);
	}
}