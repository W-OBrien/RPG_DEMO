// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	RootComponent = Collision;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ItemMesh->SetupAttachment(GetRootComponent());

	IdleParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Idle Item Particles"));
	IdleParticles->SetupAttachment(GetRootComponent());

	bIsRotating = false;
	RotationSpeed = 30.f;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnOverlapEnd);
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationSpeed;
		SetActorRotation(Rotation);
	}

}

void ABaseItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));

	if(OnOvelapParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnOvelapParticles, GetActorLocation(), FRotator(0.f), true);
	}

	if (ItemOverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, ItemOverlapSound);
	}
}

void ABaseItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
}


