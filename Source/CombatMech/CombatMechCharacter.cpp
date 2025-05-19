// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatMechCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/CombatComponent.h"
#include "Components/LockOnComponent.h"
#include "Components/PlayerActionsComponent.h"
#include "Components/StatsComponent.h"
#include "Components/TraceComponent.h"
#include "Weapon/BaseWeapon.h"


ACombatMechCharacter::ACombatMechCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>("LockOnComponent");
	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComponent");
	PlayerActionsComponent = CreateDefaultSubobject<UPlayerActionsComponent>("PlayerActionsComponent");
	TraceComponent = CreateDefaultSubobject<UTraceComponent>("TraceComponent");
	CombatComponent = CreateDefaultSubobject<UCombatComponent>("CombatComponent");

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

}


void ACombatMechCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpawnAndHolsterWeapons();
}

void ACombatMechCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACombatMechCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatMechCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatMechCharacter::Look);
		
		EnhancedInputComponent->BindAction(SprintRollAction, ETriggerEvent::Started, this, &ACombatMechCharacter::SprintRollPressed);
		EnhancedInputComponent->BindAction(SprintRollAction, ETriggerEvent::Completed, this, &ACombatMechCharacter::SprintRollReleased);

		EnhancedInputComponent->BindAction(EquipPrimaryWeaponAction, ETriggerEvent::Started, this, &ACombatMechCharacter::EquipPrimary);
		EnhancedInputComponent->BindAction(EquipSecondaryWeaponAction, ETriggerEvent::Started, this, &ACombatMechCharacter::EquipSecondary);
		EnhancedInputComponent->BindAction(EquipTertiaryWeaponAction, ETriggerEvent::Started, this, &ACombatMechCharacter::EquipTertiary);

		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ACombatMechCharacter::BlockWeapon);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ACombatMechCharacter::BlockWeapon);

		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ACombatMechCharacter::LockOn);
		//EnhancedInputComponent->BindAction(HolsterAction, ETriggerEvent::Started, this, &ACombatMechCharacter::HolsterCurrentWeaponOnly);
		
		EnhancedInputComponent->BindAction(LightAttackInputAction, ETriggerEvent::Started, this, &ACombatMechCharacter::LightComboAttack);
		EnhancedInputComponent->BindAction(HeavyAttackInputAction, ETriggerEvent::Started, this, &ACombatMechCharacter::HeavyComboAttack);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACombatMechCharacter::CrouchToggle);
		
		EnhancedInputComponent->BindAction(DrinkPotionAction, ETriggerEvent::Started, this, &ACombatMechCharacter::DrinkPotion);


		

	}
}


#pragma region MovementLogic

void ACombatMechCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACombatMechCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * LookSensX);
		AddControllerPitchInput(LookAxisVector.Y * LookSensY);
	}
}

void ACombatMechCharacter::SprintRollPressed()
{
	if(PlayerActionsComponent)
		PlayerActionsComponent->OnSprintRollPressed();
}

void ACombatMechCharacter::SprintRollReleased()
{
	if(PlayerActionsComponent)
		PlayerActionsComponent->OnSprintRollReleased();
}


void ACombatMechCharacter::CrouchToggle()
{
	if(PlayerActionsComponent)
		PlayerActionsComponent->Crouch();
}

void ACombatMechCharacter::DrinkPotion()
{
	if(StatsComponent)
		StatsComponent->DrinkPotion(0);
}

#pragma endregion 

#pragma region EquipWeaponLogic

void ACombatMechCharacter::SpawnAndHolsterWeapons()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	if (KatanaWeaponClass && !SpawnedKatana)
	{
		SpawnedKatana = GetWorld()->SpawnActor<ABaseWeapon>(KatanaWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		HolsterSelectedWeapon(1);
	}
	if (SecondaryWeaponLeftClass && !SpawnedDualLeft)
	{
		SpawnedDualLeft = GetWorld()->SpawnActor<ABaseWeapon>(SecondaryWeaponLeftClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	if (SecondaryWeaponRightClass && !SpawnedDualRight)
	{
		SpawnedDualRight = GetWorld()->SpawnActor<ABaseWeapon>(SecondaryWeaponRightClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		HolsterSelectedWeapon(2);
	}
	if (ShieldClass && !SpawnedShield)
	{
		SpawnedShield = GetWorld()->SpawnActor<ABaseWeapon>(ShieldClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	if (AxeClass && !SpawnedAxe)
	{
		SpawnedAxe = GetWorld()->SpawnActor<ABaseWeapon>(AxeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		HolsterSelectedWeapon(3);
	}
}


void ACombatMechCharacter::SwitchToWeapon(int WeaponIndex)
{
	// Ignore if same weapon is already equipped
	if (EquippedWeaponIndex == WeaponIndex)
	{
		HolsterSelectedWeapon(EquippedWeaponIndex);
		return;
	}

	// Holster current weapon if any
	if (EquippedWeaponIndex != -1)
	{
		HolsterSelectedWeapon(EquippedWeaponIndex);
	}

	
	// Equip new weapon
	EquipSelectedWeapon(WeaponIndex);
}





void ACombatMechCharacter::EquipSelectedWeapon(int WeaponIndex)
{
	switch (WeaponIndex)
	{
	case 1: // Katana
		BPEquipSword();
		break;

	case 2: // Dual Swords
		BPEquipDualKatana();
		
		break;

	case 3: // Shield + Axe
		BPEquipShieldAxe();
		
		break;
	}

	bIsEquipped = true;
	EquippedWeaponIndex = WeaponIndex;
}

void ACombatMechCharacter::HolsterSelectedWeapon(int WeaponIndex)
{
	USkeletalMeshComponent* MeshComp = GetMesh();

	if (bIsEquipped && WeaponIndex == 1)
	{
		switch (WeaponIndex)
		{
		case 1:
			
			BPUnEquipSword();
			break;
		}
		
	}
	else
	{
		switch (WeaponIndex)
		{
		case 1:
			if (!SpawnedKatana) return;
			SpawnedKatana->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, KatanaHolsterSocket);
			break;

		case 2:
			if (!SpawnedDualLeft || !SpawnedDualRight) return;
			SpawnedDualLeft->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, DualLeftHolsterSocket);
			SpawnedDualRight->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, DualRightHolsterSocket);
			break;

		case 3:
			if (!SpawnedShield || !SpawnedAxe) return;
			SpawnedShield->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, ShieldHolsterSocket);
			SpawnedAxe->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, AxeHolsterSocket);
			break;
		}
	}

	
}



void ACombatMechCharacter::BlockWeapon()
{
	if(!bIsEquipped) return;
	
	if(CombatComponent)
		CombatComponent->ToggleBlockAttack();
}


void ACombatMechCharacter::LockOn()
{
	LockOnComponent->ToggleLockOn(1000.f);
}

void ACombatMechCharacter::LightComboAttack()
{
	if(CombatComponent)
		CombatComponent->CombatAttack(0);
}

void ACombatMechCharacter::HeavyComboAttack()
{
	if(CombatComponent)
		CombatComponent->CombatAttack(1);
}



#pragma endregion

bool ACombatMechCharacter::HasEnoughStamina(float StaminaCost)
{
	return StatsComponent->Stats[EStat::Stamina] >= StaminaCost;
}

float ACombatMechCharacter::GetDamage()
{
	if (CombatComponent->bIsLightAttacking)
	{
		switch (CombatComponent->LightAttackComboCounter)
		{
		case 0:
			return LightAttackBaseDamage;

		case 1:
			return LightAttackBaseDamage + 10;

		case 2:
			return LightAttackBaseDamage + 20;

		case 3:
			return LightAttackBaseDamage + 30;

		default:
			return LightAttackBaseDamage; // fallback value
		}
	}
	else if (CombatComponent->bIsHeavyAttacking)
	{
		switch (CombatComponent->HeavyAttackComboCounter)
		{
		case 0:
			return HeavyAttackBaseDamage;

		case 1:
			return HeavyAttackBaseDamage + 10;

		case 2:
			return HeavyAttackBaseDamage + 20;

		case 3:
			return HeavyAttackBaseDamage + 40;

		default:
			return HeavyAttackBaseDamage; // fallback value
		}
	}

	return 10.f;

	
	
}