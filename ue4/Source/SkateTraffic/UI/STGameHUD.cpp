// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameHUD.h"

#include "SkateTraffic/SkateTrafficGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTGameHUD, All, All)

void ASTGameHUD::DrawHUD()
{
	Super::DrawHUD();
	
}

void ASTGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		auto GameMode = Cast<ASkateTrafficGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			GameMode->OnGameStateChanged.AddUObject(this, &ASTGameHUD::OnGameStateChanged);
		}
		
	}
}

void ASTGameHUD::OnGameStateChanged(ESTGameState State)
{
	UE_LOG(LogSTGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
	UE_LOG(LogTemp, Error, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}