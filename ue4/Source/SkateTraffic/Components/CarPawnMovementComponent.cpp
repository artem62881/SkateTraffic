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

	UpdateCurrentSpeed(DeltaTime);
}

void UCarPawnMovementComponent::IncreaseSpeed(float DeltaTime)
{
	AddVelocity(PawnOwner->GetActorForwardVector() * BaseAccel * DeltaTime);
}

void UCarPawnMovementComponent::DecreaseSpeed(float DeltaTime)
{
	if (!FVector::DotProduct(PawnOwner->GetActorForwardVector(), Velocity))
	{
		AddVelocity(-PawnOwner->GetActorForwardVector() * BaseBrakeAccel * DeltaTime);
	}
}

void UCarPawnMovementComponent::UpdateCurrentSpeed(float DeltaTime)
{
	if (PawnOwner->CheckCarsInFront() && FMath::IsNearlyZero(GetCurrentVelocity().Size(), 5.f))
	{
		SetIsStanding(true);
		return;
	}
	else 
	{
		SetIsStanding(false);
	}

	//if (FMath::IsNearlyEqual(GetCurrentVelocity().Size(), BaseVelocity) && !PawnOwner->CheckCarsInFront())
	//{
	//	return;
	//}
	if (GetCurrentVelocity().Size() > BaseVelocity || PawnOwner->CheckCarsInFront())
	{
		DecreaseSpeed(DeltaTime);
	}
	else
	{
		IncreaseSpeed(DeltaTime);
	}
	
}
