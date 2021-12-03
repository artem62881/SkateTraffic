// Fill out your copyright notice in the Description page of Project Settings.


#include "PursuerMovementComponent.h"

#include "DrawDebugHelpers.h"

void UPursuerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	checkf(GetOwner()->IsA<APursuerPawn>(), TEXT("UPursuerMovementComponent::BeginPlay() PursuerMovementComponent can be used only with APursuePawn"));
	CachedOwner = StaticCast<APursuerPawn*>(GetOwner());
	CurrentVelocity = InitialVelocity;
	GetWorld()->GetTimerManager().SetTimer(IncreaseVelocityTimer, this, &UPursuerMovementComponent::IncreaseVelocity, IncreaseVelocityTime, true);
}

void UPursuerMovementComponent::TickComponent(float DeltaTime, ELevelTick Tick,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);
	DrawDebugLine(GetWorld(), CachedOwner->GetActorLocation(), CachedOwner->GetActorLocation() + CachedOwner->GetCurrentPursuitVector() * 100.f, FColor::Red, false);
	FHitResult Hit;
	FVector Delta = CurrentVelocity * CachedOwner->GetCurrentPursuitVector() * DeltaTime;
	SafeMoveUpdatedComponent(Delta, FQuat::Identity, false, Hit);
}

void UPursuerMovementComponent::IncreaseVelocity()
{
	UE_LOG(LogTemp, Error, TEXT("Pursuer velocity: %f"), CurrentVelocity);
	CurrentVelocity += AdditionalVelocity;
}
