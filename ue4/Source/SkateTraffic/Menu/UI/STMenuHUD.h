// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API ASTMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MenuWidget;

private:

};
