// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STMenuWidget.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class SKATETRAFFIC_API USTMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SwitchCharacterButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnSwitchCharacter();
};
