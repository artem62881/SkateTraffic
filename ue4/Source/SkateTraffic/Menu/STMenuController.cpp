// Fill out your copyright notice in the Description page of Project Settings.


#include "STMenuController.h"

void ASTMenuController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
