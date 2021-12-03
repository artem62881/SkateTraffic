// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawn.h"
#include "PursuerPawn.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SKATETRAFFIC_API APursuerPawn : public ASTBasePawn
{
	GENERATED_BODY()

public:
	APursuerPawn();

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void SetPursuedActor(AActor* PursuedActor);
	
	FVector GetCurrentPursuitVector() const;

	float GetCurrentPursuedActorDistance() const;
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* Box;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UArrowComponent* Arrow;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UPursuerMovementComponent* PursuerMovement;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPursuedPlayerDestroyed(AActor* DestroyedActor);
	
	TWeakObjectPtr<class APlayerPawn> CachedPursuedPlayer;

	FVector CurrentPursuitVector = FVector::ZeroVector;
};