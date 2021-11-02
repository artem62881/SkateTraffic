// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawn.h"
#include "CarPawn.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKATETRAFFIC_API ACarPawn : public ASTBasePawn
{
	GENERATED_BODY()
	
public:
	ACarPawn();

	UFUNCTION(BlueprintCallable)
	bool CheckCarsInFront();

	UFUNCTION(BlueprintCallable)
	bool CheckCarsOnNearbyLane(int32 Direction);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	class UCarPawnMovementComponent* CarPawnMovementComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FrontCarCheckDistance = 500.f;
};
