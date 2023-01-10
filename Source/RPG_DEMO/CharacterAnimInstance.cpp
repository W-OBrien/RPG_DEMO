// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}


}

void UCharacterAnimInstance::UpdateAnimProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn != nullptr)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		
		MovementSpeed = LateralSpeed.Size();

		bIsJumping = Pawn->GetMovementComponent()->IsFalling();
	}
}
