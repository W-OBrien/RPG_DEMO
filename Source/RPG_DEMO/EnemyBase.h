// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	ES_Idle UMETA(DisplayName = "Idle"),
	ES_Chase UMETA(DisplayName = "Chase"),
	ES_Attack UMETA(DisplayName = "Attack"),
	ES_MAX UMETA(DisplayName = "DEFAULT")
};

UCLASS()
class RPG_DEMO_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy State")
	EEnemyState EnemyState;

	FORCEINLINE void SetEnemyState(EEnemyState State)
	{
		EnemyState = State;
	}

	//Sphere to detect if player is close enough to chase
	
	/// <summary>
	/// Sphere to detect if player is close enough to CHASE
	/// Size adjustable in BP
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* ChaseSphere;

	/// <summary>
	/// Sphere to detect if player is close enough to ATTACK
	/// Size adjustable in BP
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* AttackSphere;

	///Get Reference to AIController
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	APlayerCharacter* AttackTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* OnHitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* AttackSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI | Collision")
	class UBoxComponent* DamageCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI | Animations")
	class UAnimMontage* AttackMontage;


	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float MinAttackWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float MaxAttackWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	bool bIsAttackOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Combat")
	float Damage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	virtual void ChaseSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ChaseSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToPlayer(APlayerCharacter* Player);

	UFUNCTION(BlueprintCallable)
	void EnableCollision();

	UFUNCTION(BlueprintCallable)
	void DisableCollision();

	UFUNCTION()
	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI | Animations")
	bool bIsAttacking;
};
