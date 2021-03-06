// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PursuerPawn.h"
#include "SkaterPawn.h"
#include "PlayerState/STPlayerState.h"
#include "SkateTraffic/SkateTrafficTypes.h"
#include "Sound/SoundCue.h"
#include "PlayerPawn.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS(Blueprintable)
class SKATETRAFFIC_API APlayerPawn : public ASkaterPawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

	virtual void BeginPlay() override;

	void Jump();

	void AddScore(uint8 ScoreToAdd);

	//TWeakObjectPtr<ASTPlayerState> GetPlayerState() const { return PlayerState; };
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* DeathCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeathCapsuleScale = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<APursuerPawn> PursuerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsPursuerEnabled = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.f, ClampMin = 0.f))
	float PursuerSpawnTime = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.f, ClampMin = 0.f))
	float PursuerInitialDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* SuccesfulJumpEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* FailedJumpEmitter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* SuccesfulJumpSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* FailedJumpSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0, ClampMin = 0))
	int32 ScoreToJump = 10;
	
private:
	UFUNCTION()
	void OnDeathCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
	FVector InitialLocation = FVector::ZeroVector;

	TWeakObjectPtr<ASTPlayerState> PlayerState;
	
	FTimerHandle PursuerInitTimer;

	void SpawnPursuer();

	TWeakObjectPtr<APursuerPawn> CachedPursuer;
};