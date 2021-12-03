// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawnMovementComponent.h"
#include "../Pawns/CarPawn.h"

void UCarPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = StaticCast<ACarPawn*>(GetOwner());
	//PawnOwner->SetActorRelativeLocation(PawnOwner->GetActorRightVector() * GetLaneXLocationPerNum(InitialLane));
	//SetInitialValues();
}

void UCarPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCarPawnMovementComponent::SetInitialValues()
{
	float BaseTargetSpeed = FMath::RandRange(MinBaseTargetSpeed, MaxBaseTargetSpeed);
	BaseAccel = FMath::RandRange(MinBaseAccel, MaxBaseAccel);
	BaseBrakeAccel = FMath::RandRange(MinBaseBrakeAccel, MaxBaseBrakeAccel);

	SetCurrentTargetSpeed(BaseTargetSpeed);
	/*UE_LOG(LogTemp, Warning, TEXT("%s | Speed: %f"), *GetOwner()->GetName(), CurrentTargetSpeed);
	UE_LOG(LogTemp, Warning, TEXT("%s | Accel: %f"), *GetOwner()->GetName(), BaseAccel);
	UE_LOG(LogTemp, Warning, TEXT("%s | BrakeAccel: %f"), *GetOwner()->GetName(), BaseBrakeAccel);
	UE_LOG(LogTemp, Warning, TEXT("------------------------------------------------"));*/
}

float UCarPawnMovementComponent::GetCurrentTargetSpeed() const
{
	return CurrentTargetSpeed;
}

void UCarPawnMovementComponent::SetCurrentTargetSpeed(float NewTargetSpeed)
{
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
		AddVelocity(-PawnOwner->GetActorForwardVector() * BaseBrakeAccel * DeltaTime);
	}
}