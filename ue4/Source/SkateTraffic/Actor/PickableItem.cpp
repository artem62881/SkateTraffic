// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"
#include "Components/CapsuleComponent.h"
#include "SkateTraffic/Pawns/PlayerPawn.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
APickableItem::APickableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	OverlapCollapseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapCollapseSphere"));
	OverlapCollapseSphere->SetupAttachment(RootComponent);
	OverlapCollapseSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OverlapCollapseSphere->SetSimulatePhysics(true);
	OverlapCollapseSphere->SetEnableGravity(false);
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ItemMesh->SetupAttachment(OverlapCollapseSphere);
	ItemMesh->SetCollisionProfileName(TEXT("NoCollision"));
	ItemMesh->SetEnableGravity(false);
	ItemCurrentRotation = GetActorRotation();
	
	OverlapAttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapOverlapAttractionSphereSphere"));
	OverlapAttractionSphere->SetupAttachment(OverlapCollapseSphere);
	OverlapAttractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapAttractionSphere->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
	CurrentAttractionPawn = nullptr;
	if (IsValid(OverlapAttractionSphere))
	{
		OverlapAttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::OnAttractionSphereBeginOverlap);
		OverlapAttractionSphere->OnComponentEndOverlap.AddDynamic(this, &APickableItem::OnAttractionSphereEndOverlap);
	}
	
	if (IsValid(OverlapCollapseSphere))
	{
		OverlapCollapseSphere->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::OnCollapseSphereBeginOverlap);
	}

	OnItemPicked.AddDynamic(this, &APickableItem::OnItemPickedEvent);
	//OnDestroyed.AddDynamic(this, &APickableItem::OnItemPickedEvent);
}


//void APickableItem::OnConstruction(const FTransform& Transform)
//{
//	ItemMesh->SetRelativeRotation(FRotator(0.f, 0.f, ItemTilt));
//}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator DeltaRot = FRotator(0.f, MeshRotationRate * DeltaTime, 0.f);
	ItemCurrentRotation += DeltaRot;
	ItemMesh->SetRelativeRotation(ItemCurrentRotation);

	UpdateItemToPawnAttraction();
}

void APickableItem::OnAttractionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("APickableItem::OnAttractionSphereBegin 1"));
	APlayerPawn* CachedPlayer = StaticCast<APlayerPawn*>(OtherActor);
	if (IsValid(CachedPlayer)/* && Cast<UCapsuleComponent>(OtherComp) == CachedPlayer->GetCapsuleComponent()*/)
	{
		//UE_LOG(LogTemp, Warning, TEXT("APickableItem::OnAttractionSphereBegin 2"));
		CurrentAttractionPawn = StaticCast<APawn*>(CachedPlayer);
	}
}

void APickableItem::OnAttractionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerPawn* CachedPlayer = StaticCast<APlayerPawn*>(OtherActor);
	if (CachedPlayer == CurrentAttractionPawn && CurrentAttractionPawn.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("APickableItem::OnAttractionSphereEnd 2"));
		DestroyItem(CachedPlayer);
	}
}

void APickableItem::OnCollapseSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("APickableItem::OnCollapseSphereBegin 1"));
	//UE_LOG(LogTemp, Warning, TEXT("APickableItem::OnCollapseSphereBegin | %s"), *OtherComp->GetName());
	APlayerPawn* CachedPlayer = StaticCast<APlayerPawn*>(OtherActor);
	if (IsValid(CachedPlayer)/* && CurrentAttractionPawn.IsValid() || Cast<UCapsuleComponent>(OtherComp) == CachedPlayer->GetCapsuleComponent()*/)
	{
		DestroyItem(CachedPlayer);
	}
}

void APickableItem::OnItemPickedEvent_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("APickableItem::OnItemPickedEvent_Implementation()"));
}

void APickableItem::DestroyItem(APlayerPawn* InstigatorPawn)
{
	if (bIsDestroying)
	{
		return;
	}
	bIsDestroying = true;
	
	FVector FXLocation = GetActorLocation();
	if (CurrentAttractionPawn.IsValid())
	{
		FXLocation = (GetActorLocation() + CurrentAttractionPawn->GetActorLocation()) / 2;
	}	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ItemCollapseFX, FXLocation, GetActorRotation());
	
	if (OnItemPicked.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnItemPicked.Broadcast()"));
		OnItemPicked.Broadcast();
	}

	InstigatorPawn->AddScore(ItemScore);
	Destroy();
}

void APickableItem::UpdateItemToPawnAttraction()
{
	if (!CurrentAttractionPawn.IsValid())
	{
		return;
	}
	float OffsetAngleCosine = FVector::DotProduct(CurrentAttractionPawn->GetActorLocation().GetSafeNormal(), CurrentAttractionPawn->GetActorForwardVector().GetSafeNormal());
	float OffsetAngleSin = FMath::Sqrt(1 - FMath::Square(OffsetAngleCosine));
	float ModifiedOffset = ToPawnAttractionForwardOffset * OffsetAngleSin;

	FVector AttractionVector = (CurrentAttractionPawn->GetActorLocation() + CurrentAttractionPawn->GetActorForwardVector() * ModifiedOffset - GetActorLocation()).GetSafeNormal();
	AttractionForce = AttractionVector * ToPawnAttractionForceValue;
	OverlapCollapseSphere->AddForce(AttractionForce);
}

