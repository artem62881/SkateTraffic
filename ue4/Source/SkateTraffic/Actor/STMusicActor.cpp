// Fill out your copyright notice in the Description page of Project Settings.


#include "STMusicActor.h"

#include "Components/AudioComponent.h"
#include "Sound/AmbientSound.h"

// Sets default values
ASTMusicActor::ASTMusicActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASTMusicActor::BeginPlay()
{
	Super::BeginPlay();
	SpawnSoundActor();
}

void ASTMusicActor::StopSong()
{
	SoundActor->Stop();
}

void ASTMusicActor::SpawnSoundActor()
{
	FActorSpawnParameters SpawnParams;
	SoundActor = GetWorld()->SpawnActor<AAmbientSound>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	SoundActor->OnEndPlay.AddDynamic(this, &ASTMusicActor::PlayNextSong);
	if (IsValid(SoundActor))
	{
		PlayNextSong(SoundActor, EEndPlayReason::LevelTransition);
	}
}

void ASTMusicActor::PlayNextSong(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	int8 RandNum = FMath::RandRange(0, MusicPlaylist.Num() - 1);
	if (MusicPlaylist[RandNum] == CurrentSong)
	{
		 MusicPlaylist.IsValidIndex(RandNum+1) ? RandNum += 1 : RandNum -= 1;
	}

	if (!MusicPlaylist.IsValidIndex(RandNum))
	{
		RandNum = 0;
	}
	
	auto NextSong = StaticCast<USoundWave*>(MusicPlaylist[RandNum]);
	if (IsValid(NextSong))
	{
		CurrentSong = NextSong;
		SoundActor->GetAudioComponent()->SetSound(NextSong);
		SoundActor->Play();
	}
}