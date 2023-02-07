// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "EnemyBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup UMETA(DisplayName = "Pickup"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_MAX UMETA(DisplayName = "DEFAULT")
};


UCLASS()
class RPG_DEMO_API AWeapon : public ABaseItem
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	AWeapon();
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Weapon")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skeletal Mesh | Weapon")
	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SlashSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Collision")
	class UBoxComponent* WeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	float Damage;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void EquipWeapon(APlayerCharacter* Player);

	FORCEINLINE void SetWeaponState(EWeaponState State)
	{
		WeaponState = State;
	}

	FORCEINLINE EWeaponState GetWeaponState()
	{
		return WeaponState;
	}

	UFUNCTION(BlueprintCallable)
	void EnableCollision();

	UFUNCTION(BlueprintCallable)
	void DisableCollision();

};
