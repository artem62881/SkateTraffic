// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
//#include "MediaAssets/Public/MediaPlaylist.h"
#include "STGameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterSwitchedSignature);

class APlayerPawn;

UCLASS()
class SKATETRAFFIC_API USTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;
	
	FName GetStartupLevelName() const { return StartupLevelName; };
	FName GetMenuLevelName() const { return MenuLevelName; };
	
	//void SetPlayerPawnClass(TSubclassOf<APlayerPawn> NewPawnClass) { PlayerPawnClass = CNewPawnClass; };
	void SwitchPlayerCharacter();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterSwitchedSignature OnCharacterSwitched;
	
	UFUNCTION(BlueprintCallable)
	APlayerPawn* GetPlayerPawnClassDefaultObject();

protected:
	UPROPERTY(EditDefaultsOnly)
	FName StartupLevelName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly)
	FName MenuLevelName = NAME_None;
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APlayerPawn> PlayerPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<APlayerPawn>> PlayerPawnsToSelect;
};
