// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup UMETA(DisplayName = "Pickup"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_MAX UMETA(DisplayName = "DEFAULT")
};

/**
 * 
 */
UCLASS()
class RPG_DEMO_API AWeapon : public ABaseItem
{
	GENERATED_BODY()

public:

	AWeapon();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Weapon")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh | Weapon")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* EquipSound;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void EquipWeapon(APlayerCharacter* Player);

	FORCEINLINE void SetWeaponState(EWeaponState State)
	{
		WeaponState = State;
	}

	FORCEINLINE EWeaponState GetWeaponState()
	{
		return WeaponState;
	}
	
};
