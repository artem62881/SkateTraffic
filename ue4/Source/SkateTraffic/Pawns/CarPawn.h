// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/BoxComponent.h"
#include "SkateTraffic/Components/CarPawnMovementComponent.h"
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

	UBehaviorTree* GetBehaviorTree() const;
	UCarPawnMovementComponent* GetCarPawnMovementComponent() const;

	UBoxComponent* GetCollisionBox() const;
		
	UFUNCTION(BlueprintCallable)
	ASTBasePawn* CheckCarsInFront();

	UFUNCTION(BlueprintCallable)
	bool IsNearbyLaneAvailable(int32 Direction);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentlyAvailableLane();

	UFUNCTION(BlueprintCallable)
	void SwitchLane(int32 Direction);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	class UCarPawnMovementComponent* CarPawnMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* BehaviorTree;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent;
#endif
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FrontCarCheckDistance = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugEnabled = false;
};
