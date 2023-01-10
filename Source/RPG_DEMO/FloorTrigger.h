// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "FloorTrigger.generated.h"

UCLASS()
class RPG_DEMO_API AFloorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTrigger();

	// Trigger for the player to step on to move the gate
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Trigger")
	class UBoxComponent* Trigger;
	
	// Gate To move when player is in Tigger box
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor Trigger")
	class UStaticMeshComponent* Gate;

	//Initial Gate location
	UPROPERTY(BlueprintReadWrite, Category = "Floor Trigger")
	FVector InitialGateLocation;

	FTimerHandle TriggerHandle;
	
	//time before gate goes down after player step out of trigger box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Trigger")
	float TriggerTimer;

	void CloseGate();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Trigger")
	void MoveGateUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Trigger")
	void MoveGateDown();

	UFUNCTION(BlueprintCallable, Category = "Floor Trigger")
	void UpdateGateZ_Location(float z);
};
