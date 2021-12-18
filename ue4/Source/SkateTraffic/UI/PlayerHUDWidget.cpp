// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"

#include "STGameHUD.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/GameModeBase.h"
#include "SkateTraffic/Pawns/PlayerPawn.h"

int32 UPlayerHUDWidget::GetCurrentDistance() const
{	
	auto PlayerState = Cast<ASTPlayerState>(GetOwningPlayerState());
	if (IsValid(PlayerState))
	{
		return PlayerState->GetCurrentDistance();
	}
	return 0;
}

int32 UPlayerHUDWidget::GetCurrentScore() const
{
	auto PlayerState = Cast<ASTPlayerState>(GetOwningPlayerState());
	if (IsValid(PlayerState))
	{
		return PlayerState->GetCurrentScore();
	}
	return 0;
}