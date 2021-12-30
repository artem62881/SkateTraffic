// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "CarPawn.h"
#include "PursuerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../SkateTrafficTypes.h"
#include "Async/IAsyncTask.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "SkateTraffic/SkateTrafficGameMode.h"
#include "SkateTraffic/Components/SkaterPawnMovementComponent.h"

APlayerPawn::APlayerPawn()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	DeathCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DeathCapsule"));
	DeathCapsule->SetupAttachment(RootComponent);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(DeathCapsule))
	{
		DeathCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnDeathCapsuleBeginOverlap);
	}
		
	PlayerState = Cast<ASTPlayerState>(GetPlayerState());
	InitialLocation = GetActorLocation();
	
	if(GetWorld() && bIsPursuerEnabled && IsValid(PursuerPawn) && !CachedPursuer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(PursuerInitTimer, this, &APlayerPawn::SpawnPursuer, PursuerSpawnTime);
	}
}

void APlayerPawn::Jump()
{
	if (GetSkaterPawnMovementComponent()->IsFalling()) return;
	
	auto CurrentScore = ScoreToJump;
	if(PlayerState.IsValid())
	{
		CurrentScore = PlayerState->GetCurrentScore();
	}

	if(CurrentScore >= ScoreToJump)
	{
		if (PlayerState.IsValid())
		{
			PlayerState->AddScore(-ScoreToJump);
		}
		
		if (GetWorld() && SuccesfulJumpEmitter)
		{
			//FVector SpawnLocation = GetActorLocation() - GetActorUpVector() * CapsuleComponent->GetScaledCapsuleHalfHeight();
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SuccesfulJumpEmitter, SpawnLocation);
			UGameplayStatics::SpawnEmitterAttached(SuccesfulJumpEmitter, SkateMeshComponent);
			if (SuccesfulJumpSound)
			{
				UGameplayStatics::SpawnSoundAttached(SuccesfulJumpSound, SkateMeshComponent);
			}
		}
		GetSkaterPawnMovementComponent()->Jump(EJumpType::Succesful);
	}
	else
	{
		if (GetWorld() && FailedJumpEmitter)
		{
			//FVector SpawnLocation = GetActorLocation() - GetActorUpVector() * CapsuleComponent->GetScaledCapsuleHalfHeight();
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FailedJumpEmitter, SpawnLocation);
			UGameplayStatics::SpawnEmitterAttached(FailedJumpEmitter, SkateMeshComponent);
			if (FailedJumpSound)
			{
				UGameplayStatics::SpawnSoundAttached(FailedJumpSound, SkateMeshComponent);
			}
		}
		GetSkaterPawnMovementComponent()->Jump(EJumpType::Failed);
	}
}

void APlayerPawn::AddScore(uint8 ScoreToAdd)
{
	if (PlayerState.IsValid())
	{
		PlayerState->AddScore(ScoreToAdd);
	}
}

void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (PlayerState.IsValid())
	{
		PlayerState->SetCurrentDistance((GetActorLocation() - InitialLocation).Size());
	}
}

/*void APlayerPawn::OnDeathCapsuleHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("APlayerPawn::OnDeathCapsuleHitEvent 0"));
	ACarPawn* Car = Cast<ACarPawn>(OtherActor);
	if (IsValid(Car))
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerPawn::OnDeathCapsuleHitEvent 1"));
		//if (OnDeath.IsBound())
		//{
		//	OnDeath.Broadcast();
		//}
		auto GameMode = Cast<ASkateTrafficGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			GameMode->OnGameStateChanged.Broadcast(ESTGameState::GameOver);
		}
		this->Destroy();
	}
}*/

void APlayerPawn::OnDeathCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("APlayerPawn::OnDeathCapsuleBeginOverlap 0"));
	ACarPawn* Car = Cast<ACarPawn>(OtherActor);
	if (IsValid(Car))
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerPawn::OnDeathCapsuleBeginOverlap 1"));
		//if (OnDeath.IsBound())
		//{
		//	OnDeath.Broadcast();
		//}
		auto GameMode = Cast<ASkateTrafficGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(GameMode))
		{
			GameMode->OnGameStateChanged.Broadcast(ESTGameState::GameOver);
		}
		this->Destroy();
	}
}

void APlayerPawn::SpawnPursuer()
{
	FTransform PursuerInitTransform;
	PursuerInitTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	PursuerInitTransform.SetLocation(GetActorLocation() - GetActorForwardVector() * PursuerInitialDistance);

	CachedPursuer = GetWorld()->SpawnActor<APursuerPawn>(PursuerPawn, PursuerInitTransform);
	CachedPursuer->SetPursuedActor(this);
	//CachedPursuer->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	//CachedPursuer->SetOwner(this);
}
