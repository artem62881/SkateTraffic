// Fill out your copyright notice in the Description page of Project Settings.


#include "SkaterPawnAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SkateTraffic/STGameInstance.h"
#include "SkateTraffic/Components/SkaterPawnMovementComponent.h"
#include "SkateTraffic/Pawns/PlayerPawn.h"

void USkaterPawnAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	//checkf(TryGetPawnOwner()->IsA<APlayerPawn>(), TEXT("void USkaterPawnAnimInstance::NativeBeginPlay() can be used only with APlayerPawn"));
	CachedPawnOwner = StaticCast<APlayerPawn*>(TryGetPawnOwner());
}

void USkaterPawnAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (CachedPawnOwner.IsValid())
	{
		USkaterPawnMovementComponent* MovementComponent = CachedPawnOwner->GetSkaterPawnMovementComponent();
		Velocity = MovementComponent->GetCurrentVelocity().Size();
	}
	
	
	auto STGameInstance = Cast<USTGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(STGameInstance))
	{
		bOnMenuLevel = UGameplayStatics::GetCurrentLevelName(this) == STGameInstance->GetMenuLevelName().ToString();
	}
}
