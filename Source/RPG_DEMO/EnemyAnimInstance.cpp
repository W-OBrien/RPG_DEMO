// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Enemy = Cast<AEnemyBase>(Pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimProperties()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();

		if (Pawn)
		{
			Enemy = Cast<AEnemyBase>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);

		MovementSpeed = LateralSpeed.Size();

	}
}
