// Fill out your copyright notice in the Description page of Project Settings.


#include "STPlayerController.h"
#include "../SkaterPawn.h"
#include "../../SkateTrafficGameMode.h"

void ASTPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedBasePawn = Cast<ASkaterPawn>(InPawn);
}

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		const auto GameMode = Cast<ASkateTrafficGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnGameStateChanged.AddUObject(this, &ASTPlayerController::OnGameStateChanged);
		}
	}
}

void ASTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("PushForward", EInputEvent::IE_Pressed, this, &ASTPlayerController::PushForward);
	InputComponent->BindAction("SlowDown", EInputEvent::IE_Pressed, this, &ASTPlayerController::SlowDownStart);
	InputComponent->BindAction("SlowDown", EInputEvent::IE_Released, this, &ASTPlayerController::SlowDownStop);
	InputComponent->BindAction("SwitchLaneRight", EInputEvent::IE_Pressed, this, &ASTPlayerController::SwitchLaneRight);
	InputComponent->BindAction("SwitchLaneLeft", EInputEvent::IE_Pressed, this, &ASTPlayerController::SwitchLaneLeft);
	InputComponent->BindAction("PauseGame", EInputEvent::IE_Pressed, this, &ASTPlayerController::OnPauseGame);
}

void ASTPlayerController::PushForward()
{
	if (CachedBasePawn.IsValid())
	{
		CachedBasePawn->PushForwardStart();
	}
}

void ASTPlayerController::SlowDownStart()
{
	if (CachedBasePawn.IsValid())
	{
		CachedBasePawn->SlowDownStart();
	}
}

void ASTPlayerController::SlowDownStop()
{
	if (CachedBasePawn.IsValid())
	{
		CachedBasePawn->SlowDownStop();
	}
}

void ASTPlayerController::SwitchLaneRight()
{
	if (CachedBasePawn.IsValid())
	{
		CachedBasePawn->SwitchLane(1);
	}
}

void ASTPlayerController::SwitchLaneLeft()
{
	if (CachedBasePawn.IsValid())
	{
		CachedBasePawn->SwitchLane(-1);
	}
}

void ASTPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);	
}

void ASTPlayerController::OnGameStateChanged(ESTGameState State)
{
	if (State == ESTGameState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
