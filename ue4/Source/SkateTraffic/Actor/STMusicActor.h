// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STMusicActor.generated.h"

UCLASS()
class SKATETRAFFIC_API ASTMusicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTMusicActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<USoundWave*> MusicPlaylist;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
