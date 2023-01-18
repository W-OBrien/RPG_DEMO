// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PlayerCharacter.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DEMO_API ACoin : public ABaseItem
{
	GENERATED_BODY()
	
public:

	ACoin();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins")
	int32 Count;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
