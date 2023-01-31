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

	bIsAttackOverlap = false;

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

		if (Player)
		{
			SetEnemyState(EEnemyState::ES_Idle);

			AIController->StopMovement();
		}
	}
}



void AEnemyBase::AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player)
		{
			AttackTarget = Player;

			bIsAttackOverlap = true;

			SetEnemyState(EEnemyState::ES_Attack);
		}
	}
}

void AEnemyBase::AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

		if (Player)
		{
			bIsAttackOverlap = false;

			if (EnemyState != EEnemyState::ES_Attack)
			{
				MoveToPlayer(Player);
				AttackTarget = nullptr;
			}
			/*SetEnemyState(EEnemyState::ES_Chase);
			MoveToPlayer(Player);*/
		}
	}
}


void AEnemyBase::MoveToPlayer(APlayerCharacter* Player)
{
	SetEnemyState(EEnemyState::ES_Chase);

	UE_LOG(LogTemp, Warning, TEXT("Move To Player TEST"));

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Player);
		MoveRequest.SetAcceptanceRadius(30.f);

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

