// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STBasePawn.h"
#include "SkaterPawn.generated.h"

/**
 * 
 */
class USkaterPawnMovementComponent;
class UCapsuleComponent;
class UAnimMontage;
UCLASS()
class SKATETRAFFIC_API ASkaterPawn : public ASTBasePawn
{
	GENERATED_BODY()
	
public:
	ASkaterPawn();

	virtual void BeginPlay() override;

	void PushForwardStart();
	void PushForwardEnd();
	void SlowDownStart();
	void SlowDownStop();
	
	void SwitchLane(int32 Direction);

	float GetCapsuleHalfHeight();

	bool IsPushingForward() const;
	bool IsSlowingDown() const;

	USkaterPawnMovementComponent* GetSkaterPawnMovementComponent() { return SkaterPawnMovementComponent; };
	UCapsuleComponent* GetCapsuleComponent() { return CapsuleComponent; }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	USkaterPawnMovementComponent* SkaterPawnMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* CharacterMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* SkateMeshComponent;

#if	WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* PushForwardAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animations")
	UAnimMontage* SlowDownAnimMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float PushVelocity = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SlowDownVelocity = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float PushForwardDefaultDuration = 2.f;

private:
	bool bIsPushingForward = false;
	FTimerHandle PushForwardTimer;

	bool bIsSlowingDown = false;
};
