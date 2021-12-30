// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameHUD.h"
#include "SkateTraffic/SkateTrafficGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTGameHUD, All, All);

void ASTGameHUD::DrawHUD()
{
	Super::DrawHUD();
	
}

void ASTGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameStateWidgets.Add(ESTGameState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameStateWidgets.Add(ESTGameState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
	GameStateWidgets.Add(ESTGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

	for (auto GameWidgetPair : GameStateWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;
		if (!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	DrawHints();
	
	if (GetWorld())
	{
		const auto GameMode = Cast<ASkateTrafficGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnGameStateChanged.AddUObject(this, &ASTGameHUD::OnGameStateChanged);
		}
	}
}

void ASTGameHUD::OnGameStateChanged(ESTGameState State)
{
	if (CurrentStateWidget)
	{
		CurrentStateWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameStateWidgets.Contains(State))
	{
		CurrentStateWidget = GameStateWidgets[State];
		CurrentStateWidget->SetVisibility(ESlateVisibility::Visible);
	}
	//UE_LOG(LogSTGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
	//UE_LOG(LogTemp, Error, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}

void ASTGameHUD::DrawHints()
{
	HintsWidget = CreateWidget<UUserWidget>(GetWorld(), HintsWidgetClass);
	if (HintsWidget)
	{
		HintsWidget->AddToViewport();
		//if (GetWorld())
		//{
		//	GetWorld()->GetTimerManager().SetTimer(HintsTimer, this, &ASTGameHUD::RemoveHints, DrawHintsTime);
		//}
	}
}

/*void ASTGameHUD::RemoveHints()
{
	HintsWidget->SetVisibility(ESlateVisibility::Hidden);
}*/
