// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "CarPawn.h"
#include "PursuerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "../SkateTrafficTypes.h"
#include "SkateTraffic/SkateTrafficGameMode.h"
#include "SkateTraffic/Actor/PickableItem.h"

APlayerPawn::APlayerPawn()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	DeathCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DeathCapsule"));
	DeathCapsule->SetCapsuleSize(GetCapsuleComponent()->GetScaledCapsuleRadius() * DeathCapsuleScale, GetCapsuleHalfHeight());
	DeathCapsule->SetupAttachment(RootComponent);

	//DestroyItemsSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyItemsCapsule"));
	//DestroyItemsSphere->SetupAttachment(RootComponent);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(DeathCapsule))
	{
		DeathCapsule->OnComponentHit.AddDynamic(this, &APlayerPawn::OnDeathCapsuleHitEvent);
	}
	
	PlayerState = Cast<ASTPlayerState>(GetPlayerState());
	InitialLocation = GetActorLocation();
	//if (IsValid(DestroyItemsSphere))
	//{
	//	DestroyItemsSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerPawn::OnDestroyItemsSphereEndOverlap);
	//}
	
	if(bIsPursuerEnabled && IsValid(PursuerPawn) && !CachedPursuer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(PursuerInitTimer, this, &APlayerPawn::SpawnPursuer, PursuerSpawnTime);
	}
}

void APlayerPawn::AddScore(uint8 ScoreToAdd)
{
	PlayerState->AddScore(ScoreToAdd);
}

void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PlayerState->SetCurrentDistance((GetActorLocation() - InitialLocation).Size());
}

void APlayerPawn::OnDeathCapsuleHitEvent(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACarPawn* Car = Cast<ACarPawn>(OtherActor);
	if (IsValid(Car))
	{
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
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
