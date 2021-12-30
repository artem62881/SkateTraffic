// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkateTraffic/Actor/BaseTileActor.h"
#include "ObjectsSpawnComponent.generated.h"

USTRUCT(BlueprintType)
struct FObjectToSpawn
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnPeriodicity = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightOffset = 150.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnVelocity = 0.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKATETRAFFIC_API UObjectsSpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectsSpawnComponent();

	void DestroyItemsOnTile(ABaseTileActor* DestroyedTile);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "ObjectName"))
	TArray<FObjectToSpawn> ObjectsToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (TitleProperty = "ObjectName"))
    TArray<FObjectToSpawn> CarsToSpawn;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> LanesXLocations = { -450.f, 0.f, 450.f };

private:
	void UpdateSpawnedObjectsArray();
	
	void SpawnObjects(FObjectToSpawn ObjectToSpawn, ABaseTileActor* TileToSpawnIn);

	void SpawnCars(FObjectToSpawn ObjectToSpawn, ABaseTileActor* TileToSpawnIn);
	
	TWeakObjectPtr<class ATilesSpawnActor> CachedOwner;
	
	void OnTileSpawned(ABaseTileActor* SpawnedTile);

	UFUNCTION()
	void OnObjectDestroyed(AActor* DestroyedActor);
	
	UPROPERTY()
	TArray<AActor*> CurrentlySpawnedObjects;
};