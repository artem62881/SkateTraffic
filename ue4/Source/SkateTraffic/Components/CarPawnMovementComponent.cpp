// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawnMovementComponent.h"
#include "../Pawns/CarPawn.h"
#include "Kismet/KismetSystemLibrary.h"

void UCarPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = StaticCast<ACarPawn*>(GetOwner());

	
}

void UCarPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UCarPawnMovementComponent::GetBaseVelocity() const
{
	return BaseVelocity;
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