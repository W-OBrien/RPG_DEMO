// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class RPG_DEMO_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "Animation Properties")
	void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;


};
