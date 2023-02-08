// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Weapon.h"



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
	bIsLeftClick = false;
	bIsEDown = false;
	bHasEnemyTarget = false;

	//Enums Initialize
	MovementState = EMovementState::EMS_Base;
	StaminaState = EStaminaState::ESS_Base;

	StaminaRate = 5.f;
	RunMinStamina = 10.f;

	Interp = 15.f;
	bInterpToEnemy = false;
}



// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerCharController>(GetController());
	
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

	if (bInterpToEnemy && EnemyTarget)
	{
		FRotator LookAtYaw = GetYawRotation(EnemyTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, Interp);

		SetActorRotation(InterpRotation);
	}

	if (EnemyTarget)
	{
		EnemyTargetLocation = EnemyTarget->GetActorLocation();

		if (PlayerController)
		{
			PlayerController->EnemyLocation = EnemyTargetLocation;
		}
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

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &APlayerCharacter::OnLeftClick);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &APlayerCharacter::OnLeftClickRelease);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::OnEDown);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::OnERelease);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnGamepad", this, &APlayerCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpGamepad", this, &APlayerCharacter::TurnRate);

}

void APlayerCharacter::MoveForward(float axis)
{
	if ((Controller != nullptr) && (axis != 0.f) && !bIsAttacking)
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
	if ((Controller != nullptr) && (axis != 0.f) && !bIsAttacking)
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

FRotator APlayerCharacter::GetYawRotation(FVector target)
{
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target);
	FRotator LookYawRotation(0.f, LookAt.Yaw, 0.f);
	return LookYawRotation;
}

void APlayerCharacter::LowerHealth(float dmg)
{
	if (Health - dmg <= 0.f)
	{
		Health -= dmg;
		Death();
	}
	else
	{
		Health -= dmg;
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	LowerHealth(DamageAmount);
	return DamageAmount;
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

void APlayerCharacter::Death()
{
	UAnimInstance* AnimationInstance = GetMesh()->GetAnimInstance();

	if (AnimationInstance && AttackMontage)
	{
		AnimationInstance->Montage_Play(AttackMontage, 1.0f);
		AnimationInstance->Montage_JumpToSection(FName("Death"));
	}
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

void APlayerCharacter::OnLeftClick()
{
	bIsLeftClick = true;

	if (EquipedWeapon)
	{
		Attack();
	}
}

void APlayerCharacter::OnLeftClickRelease()
{
	bIsLeftClick = false;
}

void APlayerCharacter::Attack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		SetInterp(true);

		UAnimInstance* AnimationInstance = GetMesh()->GetAnimInstance();

		if (AnimationInstance && AttackMontage)
		{
			int32 Attacks = FMath::RandRange(0, 2);

			switch (Attacks)
			{
			case 0:
				AnimationInstance->Montage_Play(AttackMontage, 1.25f);
				AnimationInstance->Montage_JumpToSection(FName("Attack 1"), AttackMontage);

				break;
			case 1:
				AnimationInstance->Montage_Play(AttackMontage, 1.25f);
				AnimationInstance->Montage_JumpToSection(FName("Attack 2"), AttackMontage);

				break;
			case 2:
				AnimationInstance->Montage_Play(AttackMontage, 1.25f);
				AnimationInstance->Montage_JumpToSection(FName("Attack 3"), AttackMontage);

				break;

			default:
				break;
			}
		}
	}
}

void APlayerCharacter::AttackEnd()
{
	bIsAttacking = false;
	SetInterp(false);

	if (bIsLeftClick)
	{
		Attack();
	}
}

void APlayerCharacter::SetInterp(bool interp)
{
	bInterpToEnemy = interp;
}

void APlayerCharacter::PlaySlashSound()
{
	if (EquipedWeapon->SlashSound)
	{
		UGameplayStatics::PlaySound2D(this, EquipedWeapon->SlashSound);
	}
}


void APlayerCharacter::OnEDown()
{
	bIsEDown = true;

	if (OverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(OverlappingItem);

		if (Weapon)
		{
			Weapon->EquipWeapon(this);
			SetOverlappingItem(nullptr);
		}
	}
}

void APlayerCharacter::OnERelease()
{
	bIsEDown = false;
}


void APlayerCharacter::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (EquipedWeapon)
	{
		EquipedWeapon->Destroy();
	}

	EquipedWeapon = WeaponToSet;
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


