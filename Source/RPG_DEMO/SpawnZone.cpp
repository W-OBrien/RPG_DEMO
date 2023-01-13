// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnZone.h"

// Sets default values
ASpawnZone::ASpawnZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawning Box"));
}

// Called when the game starts or when spawned
void ASpawnZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnZone::GetSpawnPoint()
{
	FVector Extent = SpawnBox->GetScaledBoxExtent();
	FVector Origin = SpawnBox->GetComponentLocation();

	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return RandomPoint;
}

void ASpawnZone::SpawnPawn_Implementation(UClass* spawn, const FVector& location)
{
	if (spawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParam;

		if (World)
		{
			// ***CHANGE ACTOR TO ENEMY CLASS***
			ABaseItem* ItemSpawned = World->SpawnActor<ABaseItem>(spawn, location, FRotator(0.0f), SpawnParam);
		}
	}
}

