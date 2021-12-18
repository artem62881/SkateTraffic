// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentDistance() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentScore() const;
	
};
