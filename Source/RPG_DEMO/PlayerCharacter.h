// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Base UMETA(DisplayName = "Base State"),
	EMS_Sprint UMETA(DisplayName = "Sprint State"),
	EMS_MAX UMETA(DisplayName = "DEFAULT"),
};

UENUM(BlueprintType)
enum class EStaminaState : uint8
{
	ESS_Base UMETA(DisplayName = "Base Stamina State"),
	ESS_Under10 UMETA(DisplayName = "Under 10% Stamina State"),
	ESS_Depleted UMETA(DisplayName = "Depleted Stamina State"),
	ESS_Recovery UMETA(DisplayName = "Stamina Recovery State"), 
	ESS_MAX UMETA(DisplayName = "DEFAULT"),
};

UCLASS()
class RPG_DEMO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	TArray<FVector> PickupLocation;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum")
	EMovementState MovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum")
	EStaminaState StaminaState;

	FORCEINLINE void SetStaminaState(EStaminaState state)
	{
		StaminaState = state;
	}

	//Rate at wich stamina will drain while running
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Run | Stamina")
	float StaminaRate;
	
	//Minimum stamina % before the player can run
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Run | Stamina")
	float RunMinStamina;


	//Set Movement Status
	void SetMovementState(EMovementState state);

	//Base movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Run")
	float MoveSpeed;

	//Max movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Run")
	float RunSpeed;

	//Check if shift key is pressed
	bool bShiftDown;

	//Start running
	void ShiftDown();

	//Stop running
	void ShiftUp();

	// Placing camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	// Creating camera that will follow the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	// Base rates to scale turning founctions for the camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	FORCEINLINE class USpringArmComponent* GetCameraArm() const
	{
		return CameraArm;
	}
	
	FORCEINLINE class UCameraComponent* GetPlayerCamera() const
	{
		return PlayerCamera;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for Forward and Backwards movement inputs
	void MoveForward(float axis);

	// Called for Left and Right movement inputs 
	void MoveRight(float axis);

	// Call by input to turn at a specific rate
	// param "float rate" is normalized (1 = 100%)
	void TurnRate(float rate);

	// Call by input to look UP and DOWN at a specific rate
	// param "float rate" is normalized (1 = 100%)
	void LookUpRate(float rate);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	UFUNCTION()
	void TakeDamage(float dmg);

	UFUNCTION()
	void Heal(float hp);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void AddCoins(int32 coin);
};
