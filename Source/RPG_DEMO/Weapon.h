// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DEMO_API AWeapon : public ABaseItem
{
	GENERATED_BODY()

public:

	AWeapon();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh | Weapon")
	class USkeletalMeshComponent* WeaponMesh;


	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
};
