// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTileActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SkateTraffic/Pawns/PlayerPawn.h"

// Sets default values
ABaseTileActor::ABaseTileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("StartingPoint"));

	BuildingsComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Buildings"));
	BuildingsComponent->SetupAttachment(RootComponent);
	BuildingsComponent->SetMobility(EComponentMobility::Static);
	
	RoadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoadMesh"));
	RoadMesh->SetupAttachment(RootComponent);
	RoadMesh->SetMobility(EComponentMobility::Static);
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetMobility(EComponentMobility::Static);
	OverlapBox->SetCollisionProfileName("OverlapAll");


	DeathBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathBox"));
	DeathBox->SetupAttachment(RootComponent);
	DeathBox->SetMobility(EComponentMobility::Static);
	DeathBox->SetCollisionProfileName("OverlapAll");
}

UStaticMeshComponent* ABaseTileActor::GetMesh() const
{
	return RoadMesh;
}

ETileType ABaseTileActor::GetTileType() const
{
	return TileType;
}

float ABaseTileActor::GetTileAngle() const
{
	return RoadAngle;
}

float ABaseTileActor::GetTileLength() const
{
	if (TileType == ETileType::Crossroads)
	{
		return RoadMesh->GetStaticMesh()->GetBoundingBox().GetSize().Y;
	}
	else
	{
		return TileLength;
	}
}

void ABaseTileActor::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ABaseTileActor::OnBoxEndOverlap);
	DeathBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseTileActor::OnDeathBoxBeginOverlap);
}

void ABaseTileActor::OnConstruction(const FTransform& Transform)
{
	RoadAngle = TileType == ETileType::Crossroads ? 0.f : 5.f;

	if (TileType == ETileType::StraightRoad)
	{
		RoadMesh->SetRelativeLocation(FVector(0.f, TileLength/2, -TileLength/2 * UKismetMathLibrary::DegSin(RoadAngle)));
		RoadMesh->SetRelativeRotation(FRotator(0.f, 0.f, RoadAngle).Quaternion());
		RoadMesh->SetRelativeScale3D(FVector(1.f, TileLength/1000.f, 1.f));

		OverlapBox->SetRelativeLocation(FVector(0.f, TileLength + OverlapBoxOffset, OverlapBox->GetScaledBoxExtent().Z - (TileLength + OverlapBoxOffset) * UKismetMathLibrary::DegSin(RoadAngle)));
	}
	else if (TileType == ETileType::Crossroads)
	{
		RoadMesh->SetRelativeLocation(FVector(-5.f, 3360.f/2, 0.f));
		OverlapBox->SetRelativeLocation(FVector(-5.f, 3360.f + OverlapBoxOffset, OverlapBox->GetScaledBoxExtent().Z));
	}
}

void ABaseTileActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Delegate 1"));
	if (!IsOverlappingCharacterCapsule(OtherActor, OtherComp))
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Delegate 2"));
	this->Destroy();
}

void ABaseTileActor::OnDeathBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Error, TEXT("DEATH BOX"));
	OtherActor->Destroy();
}


bool ABaseTileActor::IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	//UE_LOG(LogTemp, Error, TEXT("Capsule check 1"));
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
	if (!IsValid(PlayerPawn))
	{
		return false;
	}
	//UE_LOG(LogTemp, Error, TEXT("Capsule check 2"));
	//if (Cast<UCapsuleComponent>(OtherComp) != PlayerPawn->GetCapsuleComponent())
	//{
	//	return false;
	//}
	//UE_LOG(LogTemp, Error, TEXT("Capsule check 3"));
	return true;
}
