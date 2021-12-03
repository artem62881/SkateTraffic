// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../SkateTrafficTypes.h"
#include "STGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API ASTGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PlayerHudWidget;

	virtual void BeginPlay() override;

private:
	void OnGameStateChanged(ESTGameState State);
};
