// Fill out your copyright notice in the Description page of Project Settings.


#include "SkaterPawnMovementComponent.h"
#include "../Pawns/SkaterPawn.h"

void USkaterPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(PushForwardVelocityCurve))
	{
		FOnTimelineFloatStatic PushForwardVelocityUpdate;
		PushForwardVelocityUpdate.BindUObject(this, &USkaterPawnMovementComponent::PushForwardVelocityUpdate);
		PushForwardVelocityTimeline.AddInterpFloat(PushForwardVelocityCurve, PushForwardVelocityUpdate);
	}
	PawnOwner = StaticCast<ASkaterPawn*>(GetOwner());
	SetPawnInitialVelocity(InitialVelocity * GetPawnOwner()->GetActorForwardVector());
	SetCurrentLaneNum(1);
}

void USkaterPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentPushForwardMaxVelocity = PushForwardMaxVelocity * FMath::Clamp((MaxVelocity - GetCurrentVelocity().Size()) / MaxVelocity, 0.f, 1.f);
	PushForwardUpdate(DeltaTime);
	SlowDownUpdate(DeltaTime);
}

void USkaterPawnMovementComponent::PushForwardStart()
{
	PushForwardVelocityTimeline.PlayFromStart();
}

void USkaterPawnMovementComponent::PushForwardUpdate(float DeltaTime)
{
	if (PawnOwner->IsPushingForward())
	{	
		PushForwardVelocityTimeline.TickTimeline(DeltaTime);
		AddVelocity(UpdatedComponent->GetForwardVector() * CurrentPushForwardVelocity * DeltaTime);
	}
}


void USkaterPawnMovementComponent::PushForwardEnd()
{
	PushForwardVelocityTimeline.Stop();
	CurrentPushForwardVelocity = 0.f;
}

void USkaterPawnMovementComponent::SwitchLaneStart(int32 Direction)
{
	if (PawnOwner->IsPushingForward())
	{
		PawnOwner->PushForwardEnd();
	}
	Super::SwitchLaneStart(Direction);
}

void USkaterPawnMovementComponent::PushForwardVelocityUpdate(float Alpha)
{
	CurrentPushForwardVelocity = FMath::Lerp(0.f, CurrentPushForwardMaxVelocity, Alpha);
}

void USkaterPawnMovementComponent::SlowDownUpdate(float DeltaTime)
{
	if (!PawnOwner->IsSlowingDown())
	{
		SetIsStanding(false);
	}

	if (IsStanding())
	{
		return;
	}

	if (PawnOwner->IsSlowingDown())
	{
		if (Velocity.IsNearlyZero(20.f))
		{
			SetIsStanding(true);
			//UE_LOG(LogTemp, Error, TEXT("Stop"));
		}
			AddVelocity(UpdatedComponent->GetForwardVector() * -SlowDownVelocity * DeltaTime);
		
	}
}