// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Fighter.h"
#include "Interfaces/MainPlayer.h"
#include "Logging/LogMacros.h"
#include "CombatMechCharacter.generated.h"

class UCombatComponent;
class UTraceComponent;
class UPlayerActionsComponent;
class UStatsComponent;
class ULockOnComponent;
class ABaseWeapon;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;




UCLASS(config=Game)
class ACombatMechCharacter : public ACharacter, public IMainPlayer, public IFighter
{
	GENERATED_BODY()

public:
	ACombatMechCharacter();
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetIsEquipped() const { return bIsEquipped; }
	FORCEINLINE int GetEquippedWeaponIndex() const { return EquippedWeaponIndex; }
	virtual bool HasEnoughStamina(float StaminaCost) override;
	virtual float GetDamage() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULockOnComponent* LockOnComponent;
	void LockOn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStatsComponent* StatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlayerActionsComponent* PlayerActionsComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTraceComponent* TraceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCombatComponent* CombatComponent;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CameraShakeVisual();
	

	UFUNCTION(BlueprintImplementableEvent)
	void CrouchVisual();

	UFUNCTION(BlueprintImplementableEvent)
	void SprintVisual();
	
	UFUNCTION(BlueprintImplementableEvent)
	void LockInVisual(bool bIsLockedIn);

	void LightComboAttack();
	void HeavyComboAttack();

	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* SpawnedKatana;

	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* SpawnedDualLeft;

	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* SpawnedDualRight;

	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* SpawnedShield;

	UPROPERTY(VisibleAnywhere)
	ABaseWeapon* SpawnedAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float LightAttackBaseDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float HeavyAttackBaseDamage = 20.f;

	
	

	// === Katana (Primary) ===
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Katana")
	FName KatanaHandSocket = "KatanaHandSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Katana")
	FName KatanaHolsterSocket = "KatanaHolsterSocket";

	// === Dual Blades (Secondary) ===
	UPROPERTY(EditDefaultsOnly, Category = "Combat|DualSword")
	FName DualLeftHandSocket = "SwordLeftHandSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|DualSword")
	FName DualRightHandSocket = "SwordRightHandSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|DualSword")
	FName DualLeftHolsterSocket = "LeftBladeHolsterSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|DualSword")
	FName DualRightHolsterSocket = "RightBladeHolsterSocket";

	// === Shield & Axe (Tertiary) ===
	UPROPERTY(EditDefaultsOnly, Category = "Combat|AxeShield")
	FName ShieldHandSocket = "ShieldHandSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|AxeShield")
	FName AxeHandSocket = "AxeHandSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|AxeShield")
	FName ShieldHolsterSocket = "ShieldHolsterSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Combat|AxeShield")
	FName AxeHolsterSocket = "AxeHolsterSocket";

#pragma region Combat_StateFlags

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category="Combat")
	bool bIsEquipped;

	UPROPERTY(VisibleAnywhere)
	int32 EquippedWeaponIndex = -1;

#pragma endregion
	
	
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region MovementLogic

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SprintRollPressed ();
	void SprintRollReleased();
	void CrouchToggle();


#pragma endregion

	void DrinkPotion();

#pragma region CombatLogic

	void SpawnAndHolsterWeapons();
	void EquipSelectedWeapon(int WeaponIndex);
	void HolsterSelectedWeapon(int WeaponIndex);
	void SwitchToWeapon(int WeaponIndex);
	

	FORCEINLINE void EquipPrimary()  { SwitchToWeapon(1); }
	FORCEINLINE void EquipSecondary(){ SwitchToWeapon(2); }
	FORCEINLINE void EquipTertiary() { SwitchToWeapon(3); }

	void BlockWeapon();
	int PendingWeaponIndex = -1;
	bool bIsSwitchingWeapon = false;


#pragma endregion

	UFUNCTION(BlueprintImplementableEvent)
	void BPEquipSword();

	UFUNCTION(BlueprintImplementableEvent)
	void BPEquipDualKatana();

	UFUNCTION(BlueprintImplementableEvent)
	void BPEquipShieldAxe();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BPUnEquipSword();

	

private:

	UPROPERTY(EditDefaultsOnly, Category="Settings")
	float LookSensX = 0.75f;
	
	UPROPERTY(EditDefaultsOnly, Category="Settings")
	float LookSensY = 0.75f;

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

#pragma endregion

#pragma region Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintRollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipPrimaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipSecondaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipTertiaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HoslterWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LightAttackInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HeavyAttackInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DrinkPotionAction;

#pragma endregion



	

#pragma region Combat_WeaponClasses

	UPROPERTY(EditAnywhere, Category="Combat|Katana")
	TSubclassOf<ABaseWeapon> KatanaWeaponClass;

	UPROPERTY(EditAnywhere, Category="Combat|DualSword")
	TSubclassOf<ABaseWeapon> SecondaryWeaponLeftClass;

	UPROPERTY(EditAnywhere, Category="Combat|DualSword")
	TSubclassOf<ABaseWeapon> SecondaryWeaponRightClass;

	UPROPERTY(EditAnywhere, Category="Combat|AxeShield")
	TSubclassOf<ABaseWeapon> ShieldClass;

	UPROPERTY(EditAnywhere, Category="Combat|AxeShield")
	TSubclassOf<ABaseWeapon> AxeClass;

#pragma endregion




	
};
