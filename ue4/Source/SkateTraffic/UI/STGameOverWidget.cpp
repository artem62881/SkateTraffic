// Fill out your copyright notice in the Description page of Project Settings.


#include "STGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SkateTraffic/Pawns/PlayerPawn.h"

void USTGameOverWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdatePlayerStats();
}

int32 USTGameOverWidget::GetDistance() const
{
	return Distance;
}

int32 USTGameOverWidget::GetScore() const
{
	return Score;
}

void USTGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogTemp, Error, TEXT("USTGameOverWidget::NativeOnInitialized() 0"));
	if (ResetLevelButton)
	{
		UE_LOG(LogTemp, Error, TEXT("USTGameOverWidget::NativeOnInitialized() 1"));
		ResetLevelButton->OnClicked.AddDynamic(this, &USTGameOverWidget::OnResetLevel);
	}
}

void USTGameOverWidget::UpdatePlayerStats()
{
	auto PlayerState = Cast<ASTPlayerState>(GetOwningPlayerState());
	if (IsValid(PlayerState))
	{
		Distance = PlayerState->GetCurrentDistance();
		Score = PlayerState->GetCurrentScore();
	}
}

void USTGameOverWidget::OnResetLevel()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	UE_LOG(LogTemp, Error, TEXT("USTGameOverWidget::OnResetLevel"));
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
	UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
