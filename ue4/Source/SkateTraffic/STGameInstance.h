// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "MediaAssets/Public/MediaPlaylist.h"
#include "STGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SKATETRAFFIC_API USTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FName GetStartupLevelName() const { return StartupLevelName; };
	
protected:
	UPROPERTY(EditDefaultsOnly)
	FName StartupLevelName = NAME_None;
};
