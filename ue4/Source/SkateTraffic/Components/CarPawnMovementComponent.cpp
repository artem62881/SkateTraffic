// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawnMovementComponent.h"
#include "../Pawns/CarPawn.h"
#include "Kismet/KismetSystemLibrary.h"

void UCarPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = StaticCast<ACarPawn*>(GetOwner());
	SetInitialValues();
}

void UCarPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCarPawnMovementComponent::SetInitialValues()
{
	InitialLane = FMath::RandRange(0, GetLanesLocationsArray().Num());
	
	BaseTargetSpeed = FMath::RandRange(MinBaseTargetSpeed, MaxBaseTargetSpeed);
	BaseAccel = FMath::RandRange(MinBaseAccel, MaxBaseAccel);
	BaseBrakeAccel = FMath::RandRange(MinBaseBrakeAccel, MaxBaseBrakeAccel);

	SetCurrentTargetSpeed(BaseTargetSpeed);	
}

float UCarPawnMovementComponent::GetCurrentTargetSpeed() const
{
	return CurrentTargetSpeed;
}

void UCarPawnMovementComponent::SetCurrentTargetSpeed(float NewTargetSpeed)
{
	CurrentTargetSpeed = NewTargetSpeed;
}

float UCarPawnMovementComponent::GetBaseTargetSpeed() const
{
	return BaseTargetSpeed;
}

void UCarPawnMovementComponent::IncreaseSpeed(float DeltaTime)
{
	AddVelocity(PawnOwner->GetActorForwardVector() * BaseAccel * DeltaTime);
}

void UCarPawnMovementComponent::DecreaseSpeed(float DeltaTime)
{
	if (FVector::DotProduct(PawnOwner->GetActorForwardVector(), Velocity) > 0.f)
	{
		AddVelocity(-PawnOwner->GetActorForwardVector() * BaseBrakeAccel * DeltaTime);
	}
}