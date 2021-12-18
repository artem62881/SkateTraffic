// Fill out your copyright notice in the Description page of Project Settings.


#include "CarAIController.h"
#include "../../CarPawn.h"
#include "../../../Components/CarPawnMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCarAIController, All, All)

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

	UpdateCurrentState(DeltaSeconds);
}

void ACarAIController::UpdateCurrentState(float DeltaTime)
{
	UCarPawnMovementComponent* MovementComponent = StaticCast<UCarPawnMovementComponent*>(CachedPawnOwner->GetCarPawnMovementComponent());
	if (!IsValid(MovementComponent))
	{
		return;
	}

	ASTBasePawn* ForwardPawn  = CachedPawnOwner->CheckCarsInFront();
	bool bForwardCarCheck = IsValid(ForwardPawn);

	ACarPawn* ForwardCar = Cast<ACarPawn>(ForwardPawn);
	if (IsValid(ForwardCar) && bForwardCarCheck)
	{
		TrySetNewTargetVelocity(ForwardCar);
	}
	
	if (bForwardCarCheck && FMath::IsNearlyZero(MovementComponent->GetCurrentVelocity().Size(), 5.f))
	{
		MovementComponent->SetIsStanding(true);
		return;
	}
	else 
	{
		MovementComponent->SetIsStanding(false);
	}
	
	if (MovementComponent->GetCurrentVelocity().Size() > MovementComponent->GetCurrentTargetSpeed() || bForwardCarCheck)
	{
		MovementComponent->DecreaseSpeed(DeltaTime);
	}
	else
	{
		MovementComponent->IncreaseSpeed(DeltaTime);
	}
}

void ACarAIController::TrySetNewTargetVelocity(ACarPawn* ForwardCar)
{
	UCarPawnMovementComponent* MovementComponent = StaticCast<UCarPawnMovementComponent*>(CachedPawnOwner->GetCarPawnMovementComponent());
	UCarPawnMovementComponent* ForwardCarMovementComponent = StaticCast<UCarPawnMovementComponent*>(ForwardCar->GetCarPawnMovementComponent());
	if (!IsValid(ForwardCarMovementComponent) || !IsValid(MovementComponent))
	{
		return;
	}
		
	if (FMath::IsNearlyEqual(ForwardCarMovementComponent->GetCurrentTargetSpeed(), MovementComponent->GetCurrentTargetSpeed()))
	{
		return;	
	}
		
	if (FMath::Abs(ForwardCarMovementComponent->GetCurrentTargetSpeed() - MovementComponent->GetCurrentTargetSpeed()) > 200.f)
	{
		const int32 Direction = CachedPawnOwner->GetCurrentlyAvailableLane();
		if (Direction != 0)
		{
			if (!MovementComponent->IsSwitchingLanes())
			{
				CachedPawnOwner->SwitchLane(Direction);
			}
		}
	}
	else
	{
		MovementComponent->SetCurrentTargetSpeed(ForwardCarMovementComponent->GetCurrentTargetSpeed());
		UE_LOG(LogCarAIController, Display, TEXT("CarName: %s | NewTargetSpeed: %f"), *CachedPawnOwner->GetName(), MovementComponent->GetCurrentTargetSpeed());
	}
}
