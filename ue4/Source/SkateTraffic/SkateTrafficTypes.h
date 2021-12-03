// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define ECC_CarsCheck ECC_GameTraceChannel1
#define ECC_PickableItem ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class ESTGameState : uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, ESTGameState);
