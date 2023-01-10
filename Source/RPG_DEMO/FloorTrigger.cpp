// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTrigger.h"

// Sets default values
AFloorTrigger::AFloorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	// Set customized collision for Trigger to Overlap with Player
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Trigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	Trigger->SetBoxExtent(FVector(62.f, 62.f, 32.f));

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate"));
	Gate->SetupAttachment(GetRootComponent());

	TriggerTimer = 1.f;
}


// Called when the game starts or when spawned
void AFloorTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTrigger::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &AFloorTrigger::OnOverlapEnd);

	InitialGateLocation = Gate->GetComponentLocation();

}

// Called every frame
void AFloorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	MoveGateUp();
}


void AFloorTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	GetWorldTimerManager().SetTimer(TriggerHandle, this, &AFloorTrigger::CloseGate, TriggerTimer);
}

void AFloorTrigger::UpdateGateZ_Location(float z)
{
	FVector NewLocation = InitialGateLocation;
	NewLocation.Z += z;
	Gate->SetWorldLocation(NewLocation);
}

void AFloorTrigger::CloseGate()
{
	MoveGateDown();
}


