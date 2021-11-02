// Fill out your copyright notice in the Description page of Project Settings.


#include "STBasePawnMovementComponent.h"
#include <DrawDebugHelpers.h>
#include <Kismet/KismetSystemLibrary.h>

void USTBasePawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = StaticCast<ASTBasePawn*>(GetPawnOwner());
	OnGroundHit.AddDynamic(this, &USTBasePawnMovementComponent::OnGroundHitEvent);
	SetPawnVelocity(FVector::ZeroVector);
	CachedPawnInitialLocation = UpdatedComponent->GetComponentLocation();
	
}

void USTBasePawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bEnableGravity)
	{
		FVector FloorHitNormal = FVector::ZeroVector;
		bIsFalling = !CheckFloor(DeltaTime, FloorHitNormal);
		if (!bIsFalling)
		{	
			if (bOnGroundHitCheck && OnGroundHit.IsBound())
			{
				bOnGroundHitCheck = false;
				OnGroundHit.Broadcast(FloorHitNormal);
			}
			RotatePawnTowardsGradient(FloorHitNormal);
		}
		else
		{
			bOnGroundHitCheck = true;
		}

		UpdatePawnAcceleration(DeltaTime, FloorHitNormal);
		Velocity += CurrentPawnAcceleration * DeltaTime;
	}

	if (FVector::DotProduct(UpdatedComponent->GetForwardVector(), Velocity) < 0.f || IsStanding())
	{
		SetIsStanding(true);
		Velocity = FVector::ZeroVector;
	}
	
	FVector Delta = Velocity * DeltaTime;;
	FRotator Rot = UpdatedComponent->GetComponentRotation();

	if (!Delta.IsNearlyZero(1e-6f))
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, Rot, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, DeltaTime, Delta);
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}
	}
	UpdateComponentVelocity();	

	SwitchLaneUpdate(DeltaTime);
}

void USTBasePawnMovementComponent::AddVelocity(FVector AdditionalVelocity)
{
	Velocity += AdditionalVelocity;
}

FVector USTBasePawnMovementComponent::GetCurrentVelocity() const
{
	return Velocity;
}


bool USTBasePawnMovementComponent::IsFalling() const
{
	
	return bIsFalling;
}

bool USTBasePawnMovementComponent::IsStanding() const
{
	return bIsStanding;
}

void USTBasePawnMovementComponent::SetIsStanding(bool bIn)
{
	bIsStanding = bIn;
}

void USTBasePawnMovementComponent::UpdateCurrentVelocity(float DeltaTime)
{
	Velocity += CurrentPawnAcceleration * DeltaTime;
}

void USTBasePawnMovementComponent::OnGroundHitEvent(FVector& HitNormal)
{
	if (!HitNormal.Z == 0.f)
	{
		Velocity.Z = -Velocity.Size2D() * HitNormal.Size2D() / HitNormal.Z;
	}
}

void USTBasePawnMovementComponent::UpdatePawnAcceleration(float DeltaTime, FVector FloorNormal)
{
	FVector ResultAcceleration = FVector::ZeroVector;

	FVector GravityAcceleration = GetGravityZ() * FVector::UpVector;
	FVector FloorReactionAcceleration = FVector::ZeroVector;
	if (!IsFalling())
	{
		FloorReactionAcceleration = FloorNormal * -GetGravityZ() * FloorNormal.Z;
		SurfaceFriction = FMath::Clamp((MaxVelocity - Velocity.Size()) / MaxVelocity, 0.f, 1.f);
	}
	else
	{
		SurfaceFriction = 1.f;
	}
	
	CurrentPawnAcceleration = (GravityAcceleration + FloorReactionAcceleration) * SurfaceFriction;

	//FVector ActorLocation = UpdatedComponent->GetComponentLocation();
	//DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + GravityAcceleration, FColor::Green);
	//DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + FloorReactionAcceleration, FColor::Green);
	//DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + CurrentPawnAcceleration, FColor::Red);
}

void USTBasePawnMovementComponent::RotatePawnTowardsGradient(FVector FloorNormal)
{
	FRotator Rot = FloorNormal.ToOrientationRotator();
	Rot.Pitch -= 90.f;
	if (!FMath::IsNearlyZero(FloorNormal.Size2D(), 1e-2f))
	{
		Rot.Yaw = FloorNormal.Projection().ToOrientationRotator().Yaw;
	}
	else
	{
		Rot.Yaw = UpdatedComponent->GetComponentRotation().Yaw;
	}
	UpdatedComponent->SetWorldRotation(Rot);
}

void USTBasePawnMovementComponent::SetPawnVelocity(FVector NewVelocity)
{
	Velocity = NewVelocity;
}

bool USTBasePawnMovementComponent::CheckFloor(float DeltaTime, FVector& HitNormal)
{
	FHitResult Hit;
	FVector StartLocation = UpdatedComponent->GetComponentLocation();
	float LineTraceLength = FloorCheckTraceLength + GetGravityZ() * DeltaTime;
	FVector EndLocation = StartLocation + LineTraceLength * FVector::DownVector;
	FRotator Rotation = UpdatedComponent->GetComponentRotation();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{	
		DrawDebugLine(GetWorld(), StartLocation, Hit.ImpactPoint, FColor::Green);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 16, FColor::Red);
		DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 100.f, FColor::Red, false, -1.f, (uint8)0U, 1.f);
		HitNormal = Hit.ImpactNormal;
		return true;
	}
	else
	{
		return false;
	}
	
}

void USTBasePawnMovementComponent::SwitchLaneStart(int8 Direction)
{
	if (IsFalling() || GetCurrentVelocity().Size() < SwitchLaneSpeed)
	{
		return;
	}

	int8 NewLaneNum = GetCurrentLaneNum() + Direction;
	if (!LanesXLocations.IsValidIndex(NewLaneNum))
	{
		return;
	}

	CurrentSwitchLaneInitialLocation = PawnOwner->GetActorLocation();
	FVector NewLaneLocation = FVector(CachedPawnInitialLocation.X + GetLaneXLocationPerNum(NewLaneNum), CurrentSwitchLaneInitialLocation.Y, CurrentSwitchLaneInitialLocation.Z);
	CurrentSwitchLaneVector = NewLaneLocation - CurrentSwitchLaneInitialLocation;

	bIsSwitchingLanes = true;
}

void USTBasePawnMovementComponent::SwitchLaneUpdate(float DeltaTime)
{
	if (FMath::IsNearlyEqual(PawnOwner->GetActorLocation().X, (CurrentSwitchLaneInitialLocation + CurrentSwitchLaneVector).X, 5.f))
	{
		UE_LOG(LogTemp, Error, TEXT("Delta"));
		SwitchLaneEnd();
	}

	if (IsSwitchingLanes())
	{
		FVector Delta = CurrentSwitchLaneVector * DeltaTime * SwitchLaneSpeed / CurrentSwitchLaneVector.Size();
		FRotator NewRotation = PawnOwner->GetActorRotation();
		FHitResult Hit;
		SafeMoveUpdatedComponent(Delta, NewRotation, true, Hit);
	}

}

void USTBasePawnMovementComponent::SwitchLaneEnd()
{
	bIsSwitchingLanes = false;
	CurrentSwitchLaneInitialLocation = FVector::ZeroVector;
	CurrentSwitchLaneVector = FVector::ZeroVector;
}

float USTBasePawnMovementComponent::GetLaneXLocationPerNum(int8 Num) const
{
	return LanesXLocations[Num];
}

int8 USTBasePawnMovementComponent::GetCurrentLaneNum() const
{
	for (int8 i = 0; i < LanesXLocations.Num(); ++i)
	{
		if (FMath::IsNearlyEqual(PawnOwner->GetActorLocation().X - CachedPawnInitialLocation.X, GetLaneXLocationPerNum(i), 200.f))
		{
			return i;
		}
	}
	return 1;
}

bool USTBasePawnMovementComponent::IsSwitchingLanes() const
{
	return bIsSwitchingLanes;
}