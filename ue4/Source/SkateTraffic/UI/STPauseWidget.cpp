// Fill out your copyright notice in the Description page of Project Settings.


#include "STPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool USTPauseWidget::Initialize()
{
	const auto InitStatus = Super::Initialize();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTPauseWidget::OnClearPause);
	}

	return InitStatus;
}

void USTPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}
