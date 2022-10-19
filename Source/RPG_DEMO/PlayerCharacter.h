// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class RPG_DEMO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
};
