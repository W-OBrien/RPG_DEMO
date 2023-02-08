// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharController.h"


void APlayerCharController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (EnemyHealthWidget)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, EnemyHealthWidget);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}
}

void APlayerCharController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	if (EnemyHealthBar)
	{
		FVector2D ViewportPosition;
		ProjectWorldLocationToScreen(EnemyLocation, ViewportPosition);
		ViewportPosition.Y -= 200.f;

		FVector2D SizeInVewport(200.f, 50.f);

		EnemyHealthBar->SetPositionInViewport(ViewportPosition);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInVewport);
	}
}


void APlayerCharController::ShowEnemyHealth()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerCharController::HideEnemyHealth()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}