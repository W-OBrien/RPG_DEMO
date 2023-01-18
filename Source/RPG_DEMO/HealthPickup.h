// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PlayerCharacter.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DEMO_API AHealthPickup : public ABaseItem
{
	GENERATED_BODY()
	
public:

	AHealthPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HP;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
