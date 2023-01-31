// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsAttackOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	APlayerCharacter* AttackTarget;

	UFUNCTION(BlueprintCallable)
	void MoveToPlayer(APlayerCharacter* Player);
};
