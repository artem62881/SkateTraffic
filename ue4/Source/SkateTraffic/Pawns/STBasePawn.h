// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STBasePawn.generated.h"


UCLASS(Abstract, NotBlueprintable)
class SKATETRAFFIC_API ASTBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTBasePawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	bool IsDebugDrawEnabled() const { return bIsDebugDrawEnabled; };
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDebugDrawEnabled = false;
};
