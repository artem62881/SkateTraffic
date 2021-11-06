// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn.h"
#include "Components/BoxComponent.h"
#include "../Components/CarPawnMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkateTraffic/SkateTrafficTypes.h"
#include "../SkateTrafficTypes.h"

ACarPawn::ACarPawn()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = BoxComponent;

	CarPawnMovementComponent = CreateDefaultSubobject<UCarPawnMovementComponent>(TEXT("MovementComponent"));
	CarPawnMovementComponent->SetUpdatedComponent(BoxComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
#endif
}

UBehaviorTree* ACarPawn::GetBehaviorTree() const
{
	return BehaviorTree;
}

UCarPawnMovementComponent* ACarPawn::GetCarPawnMovementComponent() const
{
	return CarPawnMovementComponent;
}

int32 ACarPawn::GetCurrentlyAvailableLane()
{
	if (IsNearbyLaneAvailable(1))
	{
		return 1;
	}
	else if (IsNearbyLaneAvailable(-1))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

ACarPawn* ACarPawn::CheckCarsInFront()
{
	FVector TraceStart = GetActorLocation();
	FVector BoxHalfSize = BoxComponent->GetScaledBoxExtent();
	FVector TraceEnd = TraceStart + GetActorForwardVector() * FrontCarCheckDistance;
	FRotator Rotation = GetActorRotation();
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_CarsCheck);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult Hit;
	
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEnd, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
	{
		if (Hit.GetActor()->IsA<ACarPawn>())
		{
			return StaticCast<ACarPawn*>(Hit.GetActor());
		}
	}
	return nullptr;
}

bool ACarPawn::IsNearbyLaneAvailable(int32 Direction)
{
	if (!GetCarPawnMovementComponent()->GetLanesLocationsArray().IsValidIndex(GetCarPawnMovementComponent()->GetCurrentLaneNum() + Direction))
	{
		return false;
	}
	
	FVector TraceStart = GetActorLocation();
	FVector TraceEndFirst = TraceStart + GetActorRightVector() * Direction * 450.f;
	FVector TraceEndSecond = TraceStart + GetActorRightVector() * Direction * 450.f + GetActorForwardVector() * FrontCarCheckDistance;
	FVector TraceEndThird = TraceStart + GetActorRightVector() * Direction * 450.f - GetActorForwardVector() * FrontCarCheckDistance;
	
	FVector BoxHalfSize = BoxComponent->GetScaledBoxExtent();
	FRotator Rotation = GetActorRotation();
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_CarsCheck);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult Hit;
	
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEndFirst, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
	{
		return false;
	}
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEndSecond, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
	{
		return false;
	}
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEndThird, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
	{
		return false;
	}
	return true;
}

void ACarPawn::SwitchLane(int32 Direction)
{
	GetCarPawnMovementComponent()->SwitchLaneStart(Direction);
}
