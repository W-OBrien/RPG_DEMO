// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponState = EWeaponState::EWS_Pickup;
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (WeaponState == EWeaponState::EWS_Pickup && OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player)
		{
			Player->SetOverlappingItem(this);
		}
	}
}

//void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
//}

void AWeapon::EquipWeapon(APlayerCharacter* Player)
{
	if (Player)
	{
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		WeaponMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* WeaponSocket = Player->GetMesh()->GetSocketByName("WeaponHandSocket");
		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(this, Player->GetMesh());

			//Player->GetEquippedWeapon()->Destroy();
			Player->SetEquippedWeapon(this);
			Player->SetOverlappingItem(nullptr);
			EWeaponState::EWS_Equipped;
		}

		if (EquipSound)
			UGameplayStatics::PlaySound2D(this, EquipSound);
	}
}
