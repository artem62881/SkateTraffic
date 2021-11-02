// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkaterPawn.h"
#include "PlayerPawn.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKATETRAFFIC_API APlayerPawn : public ASkaterPawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;
};
