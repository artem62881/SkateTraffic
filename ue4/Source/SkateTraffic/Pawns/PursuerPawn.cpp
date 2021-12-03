// Fill out your copyright notice in the Description page of Project Settings.


#include "PursuerPawn.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "SkateTraffic/Components/PursuerMovementComponent.h"
#include "PlayerPawn.h"


APursuerPawn::APursuerPawn()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(Box);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	Mesh->SetupAttachment(Box);
	Mesh->SetCollisionProfileName("NoCollision");
	
#if WITH_EDITORONLY_DATA
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PreviewArrow"));
	Arrow->SetupAttachment(Box);;
#endif

	PursuerMovement = CreateDefaultSubobject<UPursuerMovementComponent>(TEXT("MovementComponent"));
	PursuerMovement->SetUpdatedComponent(Box);
}

void APursuerPawn::BeginPlay()
{
	Super::BeginPlay();
	//CachedPursuedPlayer = Cast<APlayerPawn>(GetOwner());
	//CachedPursuedPlayer = Cast<APlayerPawn>(GetParentActor());
	Box->OnComponentBeginOverlap.AddDynamic(this, &APursuerPawn::OnBoxBeginOverlap);
}

void APursuerPawn::OnConstruction(const FTransform& Transform)
{
	//Mesh->SetRelativeLocation(FVector(0.f, 0.f, Mesh->GetStaticMesh()->GetBoundingBox().GetExtent().Z - Box->GetScaledBoxExtent().Z));
}

FVector APursuerPawn::GetCurrentPursuitVector() const
{
	return CurrentPursuitVector;
}

float APursuerPawn::GetCurrentPursuedActorDistance() const
{
	return (CachedPursuedPlayer->GetActorLocation() - GetActorLocation()).Size();
}

void APursuerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CachedPursuedPlayer.IsValid())
	{
		CurrentPursuitVector = (CachedPursuedPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	}
	else
	{
		CurrentPursuitVector = FVector::ZeroVector;
	}
	GEngine->AddOnScreenDebugMessage(2, -1.f, FColor::Red, FString::Printf(TEXT("Pursuer distance: %f"), GetCurrentPursuedActorDistance()));
}

void APursuerPawn::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Box Collided!!"));
	if (CachedPursuedPlayer == StaticCast<APlayerPawn*>(OtherActor))
	{
		CachedPursuedPlayer->Destroy();
	}
}

void APursuerPawn::SetPursuedActor(AActor* PursuedActor)
{
	CachedPursuedPlayer = StaticCast<APlayerPawn*>(PursuedActor);
	if (CachedPursuedPlayer.IsValid())
	{
		CachedPursuedPlayer->OnDestroyed.AddDynamic(this, &APursuerPawn::OnPursuedPlayerDestroyed);
	}
}

void APursuerPawn::OnPursuedPlayerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}