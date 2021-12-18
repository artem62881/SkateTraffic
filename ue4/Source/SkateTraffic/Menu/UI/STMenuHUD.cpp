// Fill out your copyright notice in the Description page of Project Settings.


#include "STMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ASTMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}
