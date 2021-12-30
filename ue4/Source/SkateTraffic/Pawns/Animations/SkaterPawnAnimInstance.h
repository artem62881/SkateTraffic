// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SkaterPawnAnimInstance.generated.h"

/**
 * 
 */
class APlayerPawn;
 
UCLASS()
class SKATETRAFFIC_API USkaterPawnAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
    
protected:	
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation");
    float Velocity = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character animation");
	bool bOnMenuLevel = false;
	
private:
    TWeakObjectPtr<APlayerPawn> CachedPawnOwner;	
        	
};
