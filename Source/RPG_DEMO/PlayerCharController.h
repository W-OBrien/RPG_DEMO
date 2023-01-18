// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_DEMO_API APlayerCharController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	//Ref to UMG asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD Widgets")
	UUserWidget* HUDOverlay;

protected:

	virtual void BeginPlay() override;

};
