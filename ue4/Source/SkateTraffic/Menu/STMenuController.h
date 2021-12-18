// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STMenuController.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API ASTMenuController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
