// Fill out your copyright notice in the Description page of Project Settings.


#include "STBasePawnMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

void USTBasePawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PawnOwner = StaticCast<ASTBasePawn*>(GetPawnOwner());
	OnGroundHit.AddDynamic(this, &USTBasePawnMovementComponent::OnGroundHitEvent);
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
			RotatePawnTowardsFloorGradient(FloorHitNormal);
		}
		else
		{
			bOnGroundHitCheck = true;
		}

		UpdatePawnAcceleration(DeltaTime, FloorHitNormal);
		Velocity += CurrentPawnAcceleration * DeltaTime;
	}
	
	if (IsStanding())
	{
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

void USTBasePawnMovementComponent::SetIsStanding(bool bIsStanding_In)
{
	bIsStanding = bIsStanding_In;
}

void USTBasePawnMovementComponent::UpdateCurrentVelocity(float DeltaTime)
{
	Velocity += CurrentPawnAcceleration * DeltaTime;
}

void USTBasePawnMovementComponent::OnGroundHitEvent(FVector& HitNormal)
{
	if (HitNormal.Z != 0.f)
	{
		Velocity.Z = -Velocity.Size2D() * HitNormal.Size2D() / HitNormal.Z;
	}
}

void USTBasePawnMovementComponent::UpdatePawnAcceleration(float DeltaTime, FVector FloorNormal)
{
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
}

void USTBasePawnMovementComponent::RotatePawnTowardsFloorGradient(FVector FloorNormal)
{
	if (!bRotateTowardsGradient)
	{
		return;
	}
	FRotator Rot = FloorNormal.ToOrientationRotator();
	Rot.Pitch -= 90.f;
	if (!FMath::IsNearlyZero(FloorNormal.Size2D(), .01f))
	{
		Rot.Yaw = FloorNormal.Projection().ToOrientationRotator().Yaw;
	}
	else
	{
		Rot.Yaw = UpdatedComponent->GetComponentRotation().Yaw;
	}
	UpdatedComponent->SetWorldRotation(Rot);
}

void USTBasePawnMovementComponent::SetPawnInitialVelocity(FVector InVelocity)
{
	SetPawnVelocity(InVelocity);
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
	//FVector EndLocation = StartLocation + FloorCheckTraceLength * (-UpdatedComponent->GetUpVector());
	FVector EndLocation = StartLocation + FloorCheckTraceLength * FVector::DownVector;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Pawn, CollisionParams))
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

void USTBasePawnMovementComponent::SwitchLaneStart(int32 Direction)
{
	if (IsFalling() || GetCurrentVelocity().Size() < MinSwitchLaneSpeed)
	{
		return;
	}

	int32 NewLaneNum = GetCurrentLaneNum() + Direction;
	if (!LanesXLocations.IsValidIndex(NewLaneNum))
	{
		return;
	}

	CurrentSwitchLaneInitialLocationX = GetLaneXLocationPerNum(GetCurrentLaneNum());
	CurrentSwitchLaneVector = UpdatedComponent->GetRightVector() * (NewLaneNum - GetCurrentLaneNum());
	SetCurrentLaneNum(NewLaneNum);
	CurrentSwitchLaneVelocity = GetCurrentVelocity().Size() * SwitchLaneVelocityRatio;
	FMath::Clamp(CurrentSwitchLaneVelocity, MinSwitchLaneSpeed, MaxSwitchLaneSpeed);
	bIsSwitchingLanes = true;
}

TArray<float> USTBasePawnMovementComponent::GetLanesLocationsArray() const
{
	return LanesXLocations;
}

void USTBasePawnMovementComponent::SwitchLaneUpdate(float DeltaTime)
{
	if (!IsSwitchingLanes())
	{
		return;
	}
	if (FMath::IsNearlyEqual(PawnOwner->GetActorLocation().X, CurrentSwitchLaneInitialLocationX + CurrentSwitchLaneVector.X * 450.f, 20.f))
	{
		SwitchLaneEnd();
	}
	
	FVector Delta = CurrentSwitchLaneVector * DeltaTime * CurrentSwitchLaneVelocity/* / CurrentSwitchLaneVector.Size()*/;
	FRotator NewRotation = PawnOwner->GetActorRotation();
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, NewRotation, true, Hit);
}

void USTBasePawnMovementComponent::SwitchLaneEnd()
{
	bIsSwitchingLanes = false;
	CurrentSwitchLaneVelocity = 0.f;
	CurrentSwitchLaneInitialLocationX = 0.f;
	CurrentSwitchLaneVector = FVector::ZeroVector;
}

float USTBasePawnMovementComponent::GetLaneXLocationPerNum(int8 Num) const
{
	return LanesXLocations[Num];
}

int8 USTBasePawnMovementComponent::GetCurrentLaneNum() const
{
	return CurrentLaneNum;
}

void USTBasePawnMovementComponent::SetCurrentLaneNum(int32 NewNum)
{
	if (LanesXLocations.IsValidIndex(NewNum))
	{
		CurrentLaneNum = NewNum;
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s | LaneNum : %i"), *GetOwner()->GetName(), CurrentLaneNum);
	}
}

bool USTBasePawnMovementComponent::IsSwitchingLanes() const
{
	return bIsSwitchingLanes;
}
