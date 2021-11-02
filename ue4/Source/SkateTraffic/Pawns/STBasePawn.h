// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STBasePawn.generated.h"

class USTBasePawnMovementComponent;
UCLASS(Abstract, NotBlueprintable)
class SKATETRAFFIC_API ASTBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTBasePawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//float GetLaneXLocationPerNum(int8 Num) const;
	//int8 GetCurrentLaneNum() const;

	//bool IsSwitchingLanes() const;

protected:	

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TArray<float> LanesXLocations = { -450.f, 0.f, 450.f };

private:
	//bool bIsSwitchingLanes = false;
};
