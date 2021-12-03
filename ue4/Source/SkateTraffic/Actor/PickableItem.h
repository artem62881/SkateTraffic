// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PickableItem.generated.h"

//DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAddScoreSignature, uint8, Score);

class USphereComponent;
UCLASS()
class SKATETRAFFIC_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//FOnAddScoreSignature OnAddScore;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void OnConstruction(const FTransform& Transform) override;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* OverlapAttractionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* OverlapCollapseSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UNiagaraSystem* ItemCollapseFX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 ItemScore = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ItemTilt = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MeshRotationRate = 60.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ToPawnAttractionForceValue = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ToPawnAttractionForwardOffset = 0.f;
	
	UFUNCTION()
	void OnAttractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnCollapseSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FRotator ItemCurrentRotation = FRotator::ZeroRotator;

	void DestroyItem();
	
	void UpdateItemToPawnAttraction();
	TWeakObjectPtr<APawn> CurrentAttractionPawn = nullptr;
	FVector AttractionForce = FVector::ZeroVector;
	
};