// Fill out your copyright notice in the Description page of Project Settings.


#include "CarPawn.h"
#include "Components/BoxComponent.h"
#include "../Components/CarPawnMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
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

bool ACarPawn::CheckCarsInFront()
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
		return true;
	}
	return false;
}

bool ACarPawn::CheckCarsOnNearbyLane(int32 Direction)
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + GetActorRightVector() * Direction * 450.f;
	FVector BoxHalfSize = BoxComponent->GetScaledBoxExtent();
	FRotator Rotation = GetActorRotation();
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_CarsCheck);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult Hit;
	
	if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), TraceStart, TraceEnd, BoxHalfSize, Rotation, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
	{
		return true;
	}
	return false;
}
