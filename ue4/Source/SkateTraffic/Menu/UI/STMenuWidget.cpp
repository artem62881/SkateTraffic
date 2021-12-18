// Fill out your copyright notice in the Description page of Project Settings.


#include "STMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "SkateTraffic/STGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTMenuWidget, All, All)

void USTMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTMenuWidget::OnStartGame);
	}
}

void USTMenuWidget::OnStartGame()
{
	if (!GetWorld()) return;

	const auto STGameInstance = Cast<USTGameInstance>(GetWorld()->GetGameInstance());
	if (!STGameInstance) return;

	if (STGameInstance->GetStartupLevelName().IsNone())
	{
		UE_LOG(LogSTMenuWidget, Error, TEXT("Level name is NONE"));
		return;
	}
	
	UGameplayStatics::OpenLevel(this, STGameInstance->GetStartupLevelName());
}
