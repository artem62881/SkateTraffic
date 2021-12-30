// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameInstance.h"
#include "Pawns/PlayerPawn.h"

void USTGameInstance::OnStart()
{
	Super::OnStart();
	//if (PlayerPawnsToSelect.IsValidIndex(0))
	//{
	//	PlayerPawnClass = PlayerPawnsToSelect[0];
	//}
}

void USTGameInstance::SwitchPlayerCharacter()
{
	int32 CurrentNum = 0;
	if (!PlayerPawnsToSelect.IsValidIndex(CurrentNum))
	{
		return;
	}
	
	if (IsValid(PlayerPawnClass))
	{
		PlayerPawnsToSelect.Find(PlayerPawnClass, CurrentNum);
	}
	
	int32 NextNum = ++CurrentNum;
	if (PlayerPawnsToSelect.IsValidIndex(NextNum))
	{
		PlayerPawnClass = PlayerPawnsToSelect[NextNum];
	}
	else
	{
		PlayerPawnClass =  PlayerPawnsToSelect[0];
	}
	
	if (OnCharacterSwitched.IsBound())
	{
		OnCharacterSwitched.Broadcast();
	}
}

APlayerPawn* USTGameInstance::GetPlayerPawnClassDefaultObject()
{
	return PlayerPawnClass->GetDefaultObject<APlayerPawn>();
}
