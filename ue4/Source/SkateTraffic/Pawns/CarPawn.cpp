// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn.h"
#include "Components/BoxComponent.h"
#include "../Components/CarPawnMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkateTraffic/SkateTrafficTypes.h"

ACarPawn::ACarPawn()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxComponent->SetCollisionProfileName(TEXT("Vehicle"));
	RootComponent = BoxComponent;

	CarPawnMovementComponent = CreateDefaultSubobject<UCarPawnMovementComponent>(TEXT("MovementComponent"));
	CarPawnMovementComponent->SetUpdatedComponent(BoxComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticlMesh"));
	StaticMesh->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
#endif
}

UCarPawnMovementComponent* ACarPawn::GetCarPawnMovementComponent() const
{
	return CarPawnMovementComponent;
}

UBoxComponent* ACarPawn::GetCollisionBox() const
{
	return BoxComponent;
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

ASTBasePawn* ACarPawn::CheckCarsInFront()
{
	FVector TraceStart = GetActorLocation();
	FVector BoxHalfSize = BoxComponent->GetScaledBoxExtent();
	FVector TraceEnd = TraceStart + GetActorForwardVector() * FrontCarCheckDistance;
	FRotator Rotation = GetActorRotation();
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_CarsCheck);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	IsDebugDrawEnabled() ? DrawDebugType = EDrawDebugTrace::ForOneFrame : DrawDebugType = EDrawDebugTrace::None;
	FHitResult Hit;
	
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEnd, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, DrawDebugType, Hit, true))
	{
		if (Hit.GetActor()->IsA<ASTBasePawn>())
		{
			return StaticCast<ASTBasePawn*>(Hit.GetActor());
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
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
	IsDebugDrawEnabled() ? DrawDebugType = EDrawDebugTrace::ForOneFrame : DrawDebugType = EDrawDebugTrace::None;
	FHitResult Hit;
	
	if (!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEndFirst, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, DrawDebugType, Hit, true))
	{
		if (!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEndSecond, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, DrawDebugType, Hit, true))
		{
			if (!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEndThird, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, DrawDebugType, Hit, true))
			{
				return true;
			}
		}
	}	
	return false;
}

void ACarPawn::SwitchLane(int32 Direction)
{
	GetCarPawnMovementComponent()->SwitchLaneStart(Direction);
}
