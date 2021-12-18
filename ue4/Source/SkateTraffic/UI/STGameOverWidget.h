// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkateTraffic/SkateTrafficTypes.h"
#include "STGameOverWidget.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class SKATETRAFFIC_API USTGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	int32 GetDistance() const;

	UFUNCTION(BlueprintCallable)
	int32 GetScore() const;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;

	virtual void NativeOnInitialized() override;
	
private:
	void UpdatePlayerStats();

	UFUNCTION()
	void OnResetLevel();
	
	int32 Distance = 0;
	int32 Score = 0;	
};
