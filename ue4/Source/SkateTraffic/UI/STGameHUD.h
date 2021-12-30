// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "../SkateTrafficTypes.h"
#include "STGameHUD.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class SKATETRAFFIC_API ASTGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> HintsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DrawHintsTime = 3.f;
	
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TMap<ESTGameState, UUserWidget*> GameStateWidgets;

	UPROPERTY()
	UUserWidget* CurrentStateWidget = nullptr;

	UPROPERTY()
	UUserWidget* HintsWidget = nullptr;
	
	void OnGameStateChanged(ESTGameState State);

	void DrawHints();
	//void RemoveHints();
	
	FTimerHandle HintsTimer;
};