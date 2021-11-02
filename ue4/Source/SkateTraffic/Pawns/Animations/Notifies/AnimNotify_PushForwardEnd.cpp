// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PushForwardEnd.h"
#include "../../SkaterPawn.h"

void UAnimNotify_PushForwardEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ASkaterPawn* PawnOwner = Cast<ASkaterPawn>(MeshComp->GetOwner());
	if (IsValid(PawnOwner))
	{
		PawnOwner->PushForwardEnd();
	}
}