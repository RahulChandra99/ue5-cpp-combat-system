// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBATMECH_API UCombatCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UPROPERTY(BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess = true))
	class ACombatMechCharacter* CombatCharacterRef;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bEquipped;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bDefense;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bCrouched;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess = true))
	bool bDrinkingPotion;
	
	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess = true))
	int EquippedWeaponIndex;
	
	
};
