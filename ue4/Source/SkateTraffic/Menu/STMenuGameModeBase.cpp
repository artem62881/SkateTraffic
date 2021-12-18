// Fill out your copyright notice in the Description page of Project Settings.


#include "STMenuGameModeBase.h"
#include "STMenuController.h"
#include "UI/STMenuHUD.h"

ASTMenuGameModeBase::ASTMenuGameModeBase()
{
	HUDClass = ASTMenuHUD::StaticClass();
	PlayerControllerClass = ASTMenuController::StaticClass();
}
