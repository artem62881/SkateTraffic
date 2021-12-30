// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/AmbientSound.h"
#include "STMusicActor.generated.h"

class USoundWave;
class AAmbientSound;

UCLASS()
class SKATETRAFFIC_API ASTMusicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTMusicActor();

	UFUNCTION(BlueprintCallable)
	void StopSong();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<USoundWave*> MusicPlaylist;

private:
	void SpawnSoundActor();

	UFUNCTION()
	void PlayNextSong(AActor* Actor, EEndPlayReason::Type EndPlayReason);
	
	AAmbientSound* SoundActor;
	USoundWave* CurrentSong;
	FTimerHandle SongTimer;
};
