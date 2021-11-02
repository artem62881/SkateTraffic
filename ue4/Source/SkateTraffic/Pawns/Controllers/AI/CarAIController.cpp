// Fill out your copyright notice in the Description page of Project Settings.


#include "CarAIController.h"

void ACarAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (IsValid(InPawn))
	{
		checkf(InPawn->IsA<ACarPawn>(), TEXT("void ACarAIController::SetPawn(APawn* InPawn) ACarAIController can posses only ACarPawn"));
		CachedPawnOwner = StaticCast<ACarPawn*>(InPawn);
		RunBehaviorTree(CachedPawnOwner->GetBehaviorTree());
	}
	else
	{
		CachedPawnOwner = NULL;
	}
}

void ACarAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACarAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCurrentSpeed(DeltaSeconds);
}

void ACarAIController::UpdateCurrentSpeed(float DeltaTime)
{
	UCarPawnMovementComponent* MovementComponent = StaticCast<UCarPawnMovementComponent*>(CachedPawnOwner->GetCarPawnMovementComponent());
	
	if (CachedPawnOwner->CheckCarsInFront() && FMath::IsNearlyZero(MovementComponent->GetCurrentVelocity().Size(), 5.f))
	{
		MovementComponent->SetIsStanding(true);
		return;
	}
	else 
	{
		MovementComponent->SetIsStanding(false);
	}

	//if (FMath::IsNearlyEqual(GetCurrentVelocity().Size(), BaseVelocity) && !PawnOwner->CheckCarsInFront())
	//{
	//	return;
	//}
	if (MovementComponent->GetCurrentVelocity().Size() > MovementComponent->GetBaseVelocity() || CachedPawnOwner->CheckCarsInFront())
	{
		MovementComponent->DecreaseSpeed(DeltaTime);
	}
	else
	{
		MovementComponent->IncreaseSpeed(DeltaTime);
	}
}
