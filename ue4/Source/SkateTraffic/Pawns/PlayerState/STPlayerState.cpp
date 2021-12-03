// Fill out your copyright notice in the Description page of Project Settings.


#include "STPlayerState.h"

#include "SkateTraffic/Pawns/PlayerPawn.h"



void ASTPlayerState::BeginPlay()
{
	Super::BeginPlay();

}

void ASTPlayerState::AddScore(uint32 ScoreToAdd)
{
	CurrentScore += ScoreToAdd;
}

void ASTPlayerState::SetCurrentDistance(uint32 NewDistance)
{
	CurrentDistance = NewDistance/100;	
}
