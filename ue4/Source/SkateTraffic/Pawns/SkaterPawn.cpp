// Fill out your copyright notice in the Description page of Project Settings.


#include "SkaterPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/ArrowComponent.h>
#include "GameFramework/PawnMovementComponent.h"
#include "../Components/SkaterPawnMovementComponent.h"
#include <Kismet/KismetSystemLibrary.h>

ASkaterPawn::ASkaterPawn()
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleSize(40.f, 100.f);
	CapsuleComponent->SetCollisionProfileName(FName("Pawn"));
	RootComponent = CapsuleComponent;

	//SkaterPawnMovementComponent = StaticCast<USkaterPawnMovementComponent*>(GetSTBasePawnMovementComponent());
	SkaterPawnMovementComponent = CreateDefaultSubobject<USkaterPawnMovementComponent>(TEXT("MovementComponent"));
	SkaterPawnMovementComponent->SetUpdatedComponent(CapsuleComponent);

	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	CharacterMeshComponent->SetupAttachment(RootComponent);

	SkateMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkateMesh"));
	SkateMeshComponent->SetupAttachment(CharacterMeshComponent, FName("FrontFootSocket"));

#if	WITH_EDITORONLY_DATA	
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);
#endif
}

void ASkaterPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASkaterPawn::PushForwardStart()
{
	if (GetSkaterPawnMovementComponent()->IsFalling() || IsPushingForward() || GetSkaterPawnMovementComponent()->IsSwitchingLanes() || GetSkaterPawnMovementComponent()->IsStanding())
	{
		return;
	}

	bIsPushingForward = true;

	UAnimInstance* AnimInstance = CharacterMeshComponent->GetAnimInstance();
	float Duration = PushForwardDefaultDuration;
	if (IsValid(AnimInstance) && IsValid(PushForwardAnimMontage))
	{
		Duration = AnimInstance->Montage_Play(PushForwardAnimMontage);
	}
	else
	{
		GetSkaterPawnMovementComponent()->PushForwardStart();
		GetWorld()->GetTimerManager().SetTimer(PushForwardTimer, this, &ASkaterPawn::PushForwardEnd, PushForwardDefaultDuration);
	}
}

void ASkaterPawn::PushForwardEnd()
{
	bIsPushingForward = false;
	UAnimInstance* AnimInstance = CharacterMeshComponent->GetAnimInstance();
	AnimInstance->Montage_Stop(0.2f, PushForwardAnimMontage);
	GetSkaterPawnMovementComponent()->PushForwardEnd();
}

void ASkaterPawn::SlowDownStart()
{
	bIsSlowingDown = true;
}

void ASkaterPawn::SlowDownStop()
{
	bIsSlowingDown = false;
	GetSkaterPawnMovementComponent()->SetIsStanding(false);
}

void ASkaterPawn::SwitchLane(int32 Direction)
{
	GetSkaterPawnMovementComponent()->SwitchLaneStart(Direction);
}

float ASkaterPawn::GetCapsuleHalfHeight()
{
	return CapsuleComponent->GetScaledCapsuleHalfHeight();
}

bool ASkaterPawn::IsPushingForward() const
{
	return bIsPushingForward;
}

bool ASkaterPawn::IsSlowingDown() const
{
	return bIsSlowingDown;
}
