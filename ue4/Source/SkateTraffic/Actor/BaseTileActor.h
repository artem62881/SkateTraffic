// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTileActor.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnTileKilled, class ABaseTileActor*);

UENUM(BlueprintType)
enum class ETileType : uint8
{
	StraightRoad,
	Crossroads
};

class UStaticMeshComponent;
UCLASS()
class SKATETRAFFIC_API ABaseTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseTileActor();

	UStaticMeshComponent* GetMesh() const;

	ETileType GetTileType() const;

	float GetTileAngle() const;

	float GetTileLength() const;

	//FOnTileKilled OnTileKilled;
	
protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* BuildingsComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* RoadMesh;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* OverlapBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* DeathBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETileType TileType = ETileType::StraightRoad;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TileLength = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OverlapBoxOffset = 500.f;
	
private:
	float RoadAngle = 0.f;

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnDeathBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool IsOverlappingCharacterCapsule(AActor* OtherActor, UPrimitiveComponent* OtherComp);
};
