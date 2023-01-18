// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"



// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character when controller rotate, only affect camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Setup character movement parameters
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	// Create Camera Arm that pulls toward the player when collision is detected
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 500.f; // Distance Between Camera and Player
	CameraArm->bUsePawnControlRotation = true; // Rotates the arm based on controller

	// Create the Player Follow Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	PlayerCamera->bUsePawnControlRotation = true;

	// Set collision capsule size
	GetCapsuleComponent()->SetCapsuleSize(40.f, 100.f);

	// Setting turn rates for inputs
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	MaxHealth = 100.f;
	Health = 100.f;

	MaxStamina = 100.f;
	Stamina = 100.f;

	Coins = 0.f;

	MoveSpeed = 600.f;
	RunSpeed = 900.f;

	bShiftDown = false;

	//Enums Initialize
	MovementState = EMovementState::EMS_Base;
	StaminaState = EStaminaState::ESS_Base;

	StaminaRate = 5.f;
	RunMinStamina = 10.f;
}



// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation() + FVector(0, 0, 100.f), 25.f,25.f, FLinearColor::Red, 5.f, 1.f);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaRate * DeltaTime;

	switch (StaminaState)
	{
	case EStaminaState::ESS_Base:
		if (bShiftDown)
		{
			if (Stamina - DeltaStamina <= RunMinStamina)
			{
				SetStaminaState(EStaminaState::ESS_Under10);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
			SetMovementState(EMovementState::EMS_Sprint);
		}
		else //Shift Up
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementState(EMovementState::EMS_Base);
		}
		break;
	case EStaminaState::ESS_Under10:
		if (bShiftDown)
		{
			if (Stamina - DeltaStamina <= 0.f)
			{
				SetStaminaState(EStaminaState::ESS_Depleted);
				Stamina = 0.f;
				SetMovementState(EMovementState::EMS_Base);
			}
			else
			{
				Stamina -= DeltaStamina;
				SetMovementState(EMovementState::EMS_Sprint);
			}
		}
		else //Shift Up
		{
			if (Stamina + DeltaStamina >= RunMinStamina)
			{
				SetStaminaState(EStaminaState::ESS_Base);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementState(EMovementState::EMS_Base);
		}
		break;
	case EStaminaState::ESS_Depleted:
		if (bShiftDown)
		{
			Stamina = 0.f;
		}
		else //Shift Up
		{
			SetStaminaState(EStaminaState::ESS_Recovery);
			Stamina += DeltaStamina;
		}
		SetMovementState(EMovementState::EMS_Base);
		break;
	case EStaminaState::ESS_Recovery:
		if (Stamina + DeltaStamina >= RunMinStamina)
		{
			SetStaminaState(EStaminaState::ESS_Base);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementState(EMovementState::EMS_Base);
		break;
	default:
		;
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent); // Validate if PlayerInputComponent is valid before executing inputs

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::ShiftDown);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::ShiftUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnGamepad", this, &APlayerCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpGamepad", this, &APlayerCharacter::TurnRate);

}

void APlayerCharacter::MoveForward(float axis)
{
	if ((Controller != nullptr) && (axis != 0.f))
	{
		// Find the forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// With the forward vector, add movement
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, axis);
	}
}

void APlayerCharacter::MoveRight(float axis)
{
	if ((Controller != nullptr) && (axis != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, axis);
	}
}


// Key press functions
void APlayerCharacter::TurnRate(float rate)
{
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpRate(float rate)
{
	AddControllerPitchInput(rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::TakeDamage(float dmg)
{
	if (Health - dmg <= 0.f)
	{
		Health -= dmg;
		Die();
	}
	else
	{
		Health -= dmg;
	}
}

void APlayerCharacter::Heal(float hp)
{
	if (Health <= MaxHealth)
	{
		Health += hp;

		if (Health >= MaxHealth)
		{
			Health = MaxHealth;
		}
	}
}

void APlayerCharacter::Die()
{

}

void APlayerCharacter::AddCoins(int32 coin)
{
	Coins += coin;
}

void APlayerCharacter::SetMovementState(EMovementState state)
{
	MovementState = state;
	if (MovementState == EMovementState::EMS_Sprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
}

void APlayerCharacter::ShiftDown()
{
	bShiftDown = true;
}

void APlayerCharacter::ShiftUp()
{
	bShiftDown = false;
}

void APlayerCharacter::ShowPickupLocation()
{
	/*for (int32 i = 0; i < PickupLocation.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocation[i], 25.f, 25.f, FLinearColor::Red, 5.f, 1.f);
	}*/

	for (FVector Location : PickupLocation)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 25.f, FLinearColor::Red, 10.f, 1.f);
	}
}


