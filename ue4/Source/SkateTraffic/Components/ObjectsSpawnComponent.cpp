// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectsSpawnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SkateTraffic/SkateTrafficTypes.h"
#include "SkateTraffic/Actor/PickableItem.h"
#include "SkateTraffic/Actor/TilesSpawnActor.h"
#include "SkateTraffic/Pawns/CarPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogObjectsSpawnComponent, All, All)

// Sets default values for this component's properties
UObjectsSpawnComponent::UObjectsSpawnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UObjectsSpawnComponent::DestroyItemsOnTile(ABaseTileActor* DestroyedTile)
{
	TArray<AActor*> Items;
	DestroyedTile->GetAttachedActors(Items);
	for (AActor* Item : Items)
	{
		if (Item->IsA<APickableItem>())
		{
			//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("Item to destroy: %s"), *Item->GetName());
			Item->Destroy();
		}
	}
}


// Called when the game starts
void UObjectsSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetOwner()->IsA<ATilesSpawnActor>(), TEXT("UObjectsSpawnComponent::UObjectsSpawnComponent() ObjectsSpawnComponent can be used only with TileSpawnActor"))
	CachedOwner = StaticCast<ATilesSpawnActor*>(GetOwner());
	CurrentlySpawnedObjects.Reset();
	CachedOwner->OnTileSpawned.AddUObject(this, &UObjectsSpawnComponent::OnTileSpawned);
}

void UObjectsSpawnComponent::UpdateSpawnedObjectsArray()
{
	for (uint8 i = 0; i < CurrentlySpawnedObjects.Num(); ++i)
	{
		if (!IsValid(CurrentlySpawnedObjects[i]))
		{
			CurrentlySpawnedObjects[i] = nullptr;
			
			for (uint8 j = i + 1; j < CurrentlySpawnedObjects.Num(); ++j)
			{
				if (CurrentlySpawnedObjects[j] != nullptr)
				{
					CurrentlySpawnedObjects.Swap(i, j);
					break;
				}
				else if (j == CurrentlySpawnedObjects.Num() - 1/* && CurrentlySpawnedObjects[j] == nullptr*/)
				{
					CurrentlySpawnedObjects.SetNum(i);
					return;
				}
			}
		}
	}	
}

void UObjectsSpawnComponent::SpawnObjects(FObjectToSpawn ObjectToSpawn, ABaseTileActor* TileToSpawnIn)
{
	float TileLength = TileToSpawnIn->GetTileLength();
	float TileAngle = TileToSpawnIn->GetTileAngle();
	
	uint8 ObjectsNum = TileLength / ObjectToSpawn.SpawnPeriodicity;

	for (uint8 i = 0; i < ObjectsNum; ++i)
	{
		uint8 RandLaneNum = FMath::RandRange(0, LanesXLocations.Num() - 1);
		float LocationX = 0.f;
		if (LanesXLocations.IsValidIndex(RandLaneNum))
		{
			LocationX = LanesXLocations[RandLaneNum];
		}
		float Distance = FMath::FRand() * TileLength;
		
		FVector SpawnLocation = TileToSpawnIn->GetActorLocation() - FVector(LocationX, Distance * UKismetMathLibrary::DegCos(TileAngle), Distance * UKismetMathLibrary::DegSin(TileAngle) - ObjectToSpawn.HeightOffset);
		FRotator SpawnRotation = FRotator::ZeroRotator;
	
		APickableItem* SpawnedObject = GetWorld()->SpawnActor<APickableItem>(ObjectToSpawn.ObjectType, SpawnLocation, SpawnRotation);
		//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("New object: %s | 1"), *SpawnedObject->GetName());
		if (IsValid(SpawnedObject))
		{
			uint32 ObjectNum = CurrentlySpawnedObjects.AddUnique(SpawnedObject);
			CurrentlySpawnedObjects[ObjectNum]->AttachToActor(TileToSpawnIn, FAttachmentTransformRules::KeepWorldTransform);
			CurrentlySpawnedObjects[ObjectNum]->SetOwner(TileToSpawnIn);
			CurrentlySpawnedObjects[ObjectNum]->OnDestroyed.AddDynamic(this, &UObjectsSpawnComponent::OnObjectDestroyed);
			//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("New object: %s | 2"), *CurrentlySpawnedObjects[ObjectNum]->GetName());
		}
	}
}

void UObjectsSpawnComponent::SpawnCars(FObjectToSpawn ObjectToSpawn, ABaseTileActor* TileToSpawnIn)
{
	FVector TileLocation = TileToSpawnIn->GetActorLocation();
	//FRotator TileRotation = TileToSpawnIn->GetActorRotation();
	float TileLength = TileToSpawnIn->GetTileLength();
	float TileAngle = TileToSpawnIn->GetTileAngle();
	uint8 ObjectsAmount = TileLength / ObjectToSpawn.SpawnPeriodicity;
		
	for (uint8 i = 0; i < ObjectsAmount; ++i)
	{
		uint8 RandLaneNum = FMath::RandRange(0, LanesXLocations.Num() - 1);
		float LocationX = 0.f;
		if (LanesXLocations.IsValidIndex(RandLaneNum))
		{
			LocationX = LanesXLocations[RandLaneNum];
		}
		float Distance = FMath::FRand() * TileLength;
		
		FVector SpawnLocation = TileLocation + FVector(LocationX, -Distance * UKismetMathLibrary::DegCos(TileAngle), -Distance * UKismetMathLibrary::DegSin(TileAngle) + ObjectToSpawn.HeightOffset);
		FRotator SpawnRotation = FRotator(0.f, -90.f, 0.f);
		//FRotator SpawnRotation = TileRotation;
		
		FVector CarBoxExtent = FVector(1.f, 1.f, 1.f);
		ACarPawn* DefaultCar = StaticCast<ACarPawn*>(ObjectToSpawn.ObjectType->GetDefaultObject());
		if (IsValid(DefaultCar))
		{
			CarBoxExtent = DefaultCar->GetCollisionBox()->GetScaledBoxExtent();
		}
		
		ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_CarsCheck);
		TArray<AActor*> ActorsToIgnore;
		FHitResult Hit;
		if (UKismetSystemLibrary::BoxTraceSingle(GetWorld(), SpawnLocation, SpawnLocation, CarBoxExtent, SpawnRotation, TraceType, true, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true))
		{
			//UE_LOG(LogTemp, Error, TEXT("SKIP CAR SPAWN"));
			continue;
		}
		
		ACarPawn* SpawnedCar = GetWorld()->SpawnActor<ACarPawn>(ObjectToSpawn.ObjectType, SpawnLocation, SpawnRotation);
		//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("New car: %s | 1"), *SpawnedCar->GetName());
		if (IsValid(SpawnedCar))
		{
			SpawnedCar->GetCarPawnMovementComponent()->SetCurrentLaneNum(RandLaneNum);
			uint8 Num = CurrentlySpawnedObjects.AddUnique(SpawnedCar);
			CurrentlySpawnedObjects[Num]->OnDestroyed.AddDynamic(this, &UObjectsSpawnComponent::OnObjectDestroyed);
			//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("New car: %s | 2"), *CurrentlySpawnedObjects[Num]->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("%i: Name: %s | SpawnLocation : %s | RealLocation: %s"), i, *SpawnedCar->GetName(), *SpawnLocation.ToString(), *SpawnedCar->GetActorLocation().ToString());
		}
	}
}

void UObjectsSpawnComponent::OnTileSpawned(ABaseTileActor* SpawnedTile)
{
	UpdateSpawnedObjectsArray();
	
	for (FObjectToSpawn Object : ObjectsToSpawn)
	{
		//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("1. Object type ready to spawn: %s"), *Object.ObjectType->GetName());
		if (Object.ObjectType == nullptr)
		{
			return;
		}

		if (Object.ObjectType->IsChildOf<APickableItem>())
		{
			//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("2. Object type ready to spawn: %s"), *Object.ObjectType->GetName());
			SpawnObjects(Object, SpawnedTile);
		}

		if (Object.ObjectType->IsChildOf<ACarPawn>() && SpawnedTile->GetTileType() != ETileType::Crossroads)
		{
			//UE_LOG(LogObjectsSpawnComponent, Display, TEXT("2. Object type ready to spawn: %s"), *Object.ObjectType->GetName());
			SpawnCars(Object, SpawnedTile);
		}
	}
	/*for (uint8 i = 0; i < CurrentlySpawnedObjects.Num(); ++i)
	{
		if (CurrentlySpawnedObjects[i] != nullptr)
		{
			UE_LOG(LogObjectsSpawnComponent, Display, TEXT("Objects array %i: %s"), i, *CurrentlySpawnedObjects[i]->GetName());
		}
		else
		{
			UE_LOG(LogObjectsSpawnComponent, Display, TEXT("Objects array %i: NULL"), i);
		}
	}*/
	UE_LOG(LogObjectsSpawnComponent, Display, TEXT("----------------------------------------------"));
}

void UObjectsSpawnComponent::OnObjectDestroyed(AActor* DestroyedActor)
{
	//UE_LOG(LogTemp, Error, TEXT("UObjectsSpawnComponent::OnObjectDestroyed 1 | %s"), *DestroyedActor->GetName());
	if (IsValid(CurrentlySpawnedObjects[CurrentlySpawnedObjects.Find(DestroyedActor)]))
	{
		CurrentlySpawnedObjects[CurrentlySpawnedObjects.Find(DestroyedActor)] = nullptr;
	}
}
