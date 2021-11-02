// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateTrafficGameMode.h"
#include "SkateTrafficHud.h"
#include "Pawns/SkaterPawn.h"

ASkateTrafficGameMode::ASkateTrafficGameMode()
{
	DefaultPawnClass = ASkaterPawn::StaticClass();
	HUDClass = ASkateTrafficHud::StaticClass();
}
