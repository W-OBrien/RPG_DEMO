// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Combat Collision"));
	WeaponCollision->SetupAttachment(GetRootComponent());

	Damage = 50.f;

	WeaponState = EWeaponState::EWS_Pickup;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBladeOverlapBegin);
	WeaponCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnBladeOverlapEnd);

	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
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

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

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


void AWeapon::OnBladeOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);

		if (Enemy)
		{
			if (Enemy->OnHitParticles)
			{
				const USkeletalMeshSocket* BladeSocket = WeaponMesh->GetSocketByName("Blade Socket");

				if (BladeSocket)
				{
					FVector Socket = BladeSocket->GetSocketLocation(WeaponMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->OnHitParticles, Socket, FRotator(0.f), false);
				}
			}

			if (Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}
		}
	}
}

void AWeapon::OnBladeOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::EnableCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DisableCollision()
{
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}