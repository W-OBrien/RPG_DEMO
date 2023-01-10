// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawning Box"));

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawnZone->GetScaledBoxExtent();
	FVector Origin = SpawnZone->GetComponentLocation();

	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return RandomPoint;
}

