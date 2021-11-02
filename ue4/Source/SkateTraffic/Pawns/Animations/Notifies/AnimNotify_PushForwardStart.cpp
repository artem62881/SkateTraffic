// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PushForwardStart.h"
#include "../../SkaterPawn.h"
#include "../../../Components/SkaterPawnMovementComponent.h"

void UAnimNotify_PushForwardStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ASkaterPawn* PawnOwner = Cast<ASkaterPawn>(MeshComp->GetOwner());
	if (IsValid(PawnOwner))
	{
		PawnOwner->GetSkaterPawnMovementComponent()->PushForwardStart();
	}
}