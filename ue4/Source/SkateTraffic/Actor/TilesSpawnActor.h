// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTileActor.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TilesSpawnActor.generated.h"

class ABaseTileActor;

UCLASS()
class SKATETRAFFIC_API ATilesSpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATilesSpawnActor();
	
	//virtual void OnConstruction(const FTransform& Transform) override;
	
	void SpawnTiles();
	
	TArray<ABaseTileActor*> GetCurrentlySpawnedTiles() const;

	bool IsTileArrayFull() const;

	TSubclassOf<ABaseTileActor> GetTileToSpawn(uint8 TileIndex) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* PreviewMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* PreviewArrow;
#endif
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 SpawnedTilesMaxNum = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<ABaseTileActor>> TilesToSpawn;

private:
	TArray<ABaseTileActor*> CurrentlySpawnedTiles;
	
	UFUNCTION()
	void OnTileDestroyed(AActor* DestroyedActor);

	void UpdateTilesArray();

};