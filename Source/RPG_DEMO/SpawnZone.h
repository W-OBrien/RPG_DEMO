// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "SpawnZone.generated.h"

UCLASS()
class RPG_DEMO_API ASpawnZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnZone();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	class UBoxComponent* SpawnBox;

	//spawn a pawn in volume
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	//TSubclassOf<include class to spawn> SpawnPawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UKismetMathLibrary* KMath;

	UFUNCTION(BlueprintPure, Category = "Spawn")
	FVector GetSpawnPoint();

};
