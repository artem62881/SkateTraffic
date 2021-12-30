// Fill out your copyright notice in the Description page of Project Settings.


#include "TilesSpawnActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATilesSpawnActor::ATilesSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Static);

#if WITH_EDITORONLY_DATA
	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(RootComponent);

	PreviewArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PreviewArrow"));
	PreviewArrow->SetupAttachment(PreviewMesh);
	FVector ArrowOffset = FVector::UpVector * 100.f;
	PreviewArrow->SetRelativeLocation(ArrowOffset);	
#endif

	ObjectsSpawnComponent = CreateDefaultSubobject<UObjectsSpawnComponent>(TEXT("ObjectsSpawnComponent"));
}

TSubclassOf<ABaseTileActor> ATilesSpawnActor::GetTileToSpawn(uint8 TileIndex) const
{
	uint8 Num = TilesToSpawn.Num();
	if (Num == 0)
	{
		return nullptr;
	}

	if (!GetCurrentlySpawnedTiles().IsValidIndex(TileIndex))
	{
		return nullptr;
	}
	
	uint8 RandNum = UKismetMathLibrary::RandomIntegerInRange(0, Num - 1);
	if (!TilesToSpawn.IsValidIndex(RandNum))
	{
		RandNum = 0;
	}
	
	if (GetCurrentlySpawnedTiles().IsValidIndex(TileIndex - 1) && GetCurrentlySpawnedTiles()[TileIndex-1]->GetTileType() == ETileType::Crossroads)
	{
		for (TSubclassOf<ABaseTileActor> TileToSpawn : TilesToSpawn)
		{
			if (TileToSpawn.GetDefaultObject()->GetTileType() == ETileType::StraightRoad)
			{
				RandNum = TilesToSpawn.IndexOfByKey(TileToSpawn);
				break;
			}
		}
	}

	if (TileIndex == 0)
	{
		for (TSubclassOf<ABaseTileActor> TileToSpawn : TilesToSpawn)
		{
			if (TileToSpawn.GetDefaultObject()->GetTileType() == ETileType::Crossroads)
			{
				RandNum = TilesToSpawn.IndexOfByKey(TileToSpawn);
				break;
			}
		}
	}
	
	return TilesToSpawn[RandNum];
}

// Called when the game starts or when spawned
void ATilesSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentlySpawnedTiles.Reserve(SpawnedTilesMaxNum);
	CurrentlySpawnedTiles.SetNumZeroed(SpawnedTilesMaxNum);
	SpawnTiles();

#if WITH_EDITORONLY_DATA
	if (IsValid(PreviewMesh) && IsValid(PreviewArrow))
	{
		PreviewArrow->DestroyComponent();
		PreviewMesh->DestroyComponent();
	}
#endif
}

void ATilesSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATilesSpawnActor::SpawnTiles()
{
	UpdateTilesArray();
	if (IsTileArrayFull())
	{
		return;
	}
	
	for (uint8 i = 0; i < GetCurrentlySpawnedTiles().Num(); ++i)
	{
		if (GetCurrentlySpawnedTiles()[i] == nullptr)
		{
			TSubclassOf<ABaseTileActor> TileToSpawn = GetTileToSpawn(i);
			if (!IsValid(TileToSpawn))
			{
				return;
			}
	
			FTransform SpawnTransform;
			if (i > 0)
			{
				ABaseTileActor* PreviousTile = Cast<ABaseTileActor>(GetCurrentlySpawnedTiles()[i-1]);
				if (!IsValid(PreviousTile))
				{
					break;
				}
				SpawnTransform.SetLocation(PreviousTile->GetMesh()->GetSocketLocation(FName("TileAttachSocket")));
				SpawnTransform.SetRotation(PreviousTile->GetActorQuat());
			}
			else
			{
				
				SpawnTransform.SetLocation(GetActorLocation());
				SpawnTransform.SetRotation(GetActorQuat());
			}
			
			CurrentlySpawnedTiles[i] = GetWorld()->SpawnActor<ABaseTileActor>(TileToSpawn, SpawnTransform);
			CurrentlySpawnedTiles[i]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
			CurrentlySpawnedTiles[i]->SetOwner(this);
			CurrentlySpawnedTiles[i]->OnDestroyed.AddDynamic(this, &ATilesSpawnActor::OnTileDestroyed);
			if (OnTileSpawned.IsBound())
			{
				OnTileSpawned.Broadcast(CurrentlySpawnedTiles[i]);
			}
		}
	}
}

TArray<ABaseTileActor*> ATilesSpawnActor::GetCurrentlySpawnedTiles() const
{
	return CurrentlySpawnedTiles;
}

bool ATilesSpawnActor::IsTileArrayFull() const
{
	for (uint8 i = 0; i < GetCurrentlySpawnedTiles().Num(); ++i)
	{
		if (GetCurrentlySpawnedTiles()[i] == nullptr)
		{
			return false;
		}
	}
	return true;
}

void ATilesSpawnActor::OnTileDestroyed(AActor* DestroyedActor)
{
	ABaseTileActor* DestroyedTile = StaticCast<ABaseTileActor*>(DestroyedActor);
	if (!IsValid(DestroyedTile))
	{
		return;
	}

	ObjectsSpawnComponent->DestroyItemsOnTile(DestroyedTile);
	
	if (GetCurrentlySpawnedTiles().IsValidIndex(GetCurrentlySpawnedTiles().Find(DestroyedTile)) && IsValid(GetCurrentlySpawnedTiles()[GetCurrentlySpawnedTiles().Find(DestroyedTile)]))
	{
		CurrentlySpawnedTiles[GetCurrentlySpawnedTiles().Find(DestroyedTile)] = nullptr;
	}
	
	SpawnTiles();
}

void ATilesSpawnActor::UpdateTilesArray()
{
	if (IsTileArrayFull())
	{
		return;
	}
	
	for (uint8 i = 0; i < GetCurrentlySpawnedTiles().Num(); ++i)
	{
		if (GetCurrentlySpawnedTiles()[i] == nullptr)
		{
			for (uint8 j = i + 1; j < GetCurrentlySpawnedTiles().Num(); ++j)
			{
				if (GetCurrentlySpawnedTiles()[j] != nullptr)
				{
					CurrentlySpawnedTiles.Swap(i, j);
					break;
				}
				else if (j == GetCurrentlySpawnedTiles().Num() - 1 && GetCurrentlySpawnedTiles()[j] == nullptr)
				{
					return;
				}
			}
		}
	}	
}
