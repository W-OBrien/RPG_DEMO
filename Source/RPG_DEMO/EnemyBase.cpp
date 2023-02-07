// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChaseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Chase Sphere"));
	ChaseSphere->SetupAttachment(GetRootComponent());
	ChaseSphere->InitSphereRadius(1000.f);

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Sphere"));
	AttackSphere->SetupAttachment(GetRootComponent());
	AttackSphere->InitSphereRadius(100.f);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Combat Collision"));
	DamageCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("AttackSocket"));

	bIsAttackOverlap = false;

	Health = 150.f;
	MaxHealth = 150.f;
	Damage = 20.f;

	MinAttackWaitTime = 1.f;
	MaxAttackWaitTime = 2.5f;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	ChaseSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::ChaseSphereOnOverlapBegin);
	ChaseSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::ChaseSphereOnOverlapEnd);

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::AttackSphereOnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::AttackSphereOnOverlapEnd);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnAttackOverlapBegin);
	DamageCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::OnAttackOverlapEnd);

	DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::ChaseSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player)
		{
			MoveToPlayer(Player);
		}
	}
}

void AEnemyBase::ChaseSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		SetEnemyState(EEnemyState::ES_Idle);

		if (Player)
		{

			AIController->StopMovement();
		}
	}
}



void AEnemyBase::AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		SetEnemyState(EEnemyState::ES_Attack);

		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack Sphere OnOverlapBegin"));
			bIsAttackOverlap = true;
			AttackTarget = Player;

			Player->SetTarget(this);

			Attack();
		}
	}
}

void AEnemyBase::AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		SetEnemyState(EEnemyState::ES_Chase);

		if (Player)
		{
			if (Player->EnemyTarget == this)
			{
				Player->SetTarget(nullptr);
			}

			bIsAttackOverlap = false;
			
			if (EnemyState != EEnemyState::ES_Attack)
			{
				UE_LOG(LogTemp, Warning, TEXT("EnemyState != EEnemyState::ES_Attack"));

				MoveToPlayer(Player);
				AttackTarget = nullptr;
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AEnemyBase::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player)
		{
			//Add particle effects when the player gets hit
			/*if (Player->OnHitParticles)
			{
				const USkeletalMeshSocket* AttackSocket = GetMesh()->GetSocketByName("AttackSocket");

				if (AttackSocket)
				{
					FVector SocketLocation = AttackSocket->GetSocketLocation(AttackSocket);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Player->OnHitParticles, SocketLocation, FRotator(0.f), false);
				}
			}*/

			if (Player->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Player->HitSound);
			}
			
			if (DamageType)
			{
				UGameplayStatics::ApplyDamage(Player, Damage, AIController, this, DamageType);
			}
		}
	}
}

void AEnemyBase::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemyBase::MoveToPlayer(APlayerCharacter* Player)
{
	SetEnemyState(EEnemyState::ES_Chase);

	//UE_LOG(LogTemp, Warning, TEXT("Move To Player TEST"));

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Player);
		MoveRequest.SetAcceptanceRadius(20.f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		//auto PathPoints = NavPath->GetPathPoints();
		
		//Draw debug spheres to show the enemy path towards the player
		/*for (auto Point : PathPoints)
		{
			FVector Location = Point.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 25.f, FLinearColor::Red, 10.f, 1.f);
		}*/
	}
}

void AEnemyBase::EnableCollision()
{
	DamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemyBase::DisableCollision()
{
	DamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBase::Attack()
{
	if (AIController)
	{
		AIController->StopMovement();
		SetEnemyState(EEnemyState::ES_Attack);
	}

	if (!bIsAttacking)
	{
		bIsAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage, 1.25f);
			AnimInstance->Montage_JumpToSection(FName("Attack"), AttackMontage);
		}

		if (AttackSound)
		{
			UGameplayStatics::PlaySound2D(this, AttackSound);
		}
	}
}

void AEnemyBase::AttackEnd()
{
	bIsAttacking = false;

	if (bIsAttackOverlap)
	{
		float AttackWaitTime = FMath::FRandRange(MinAttackWaitTime, MaxAttackWaitTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyBase::Attack, AttackWaitTime);
	}
}

