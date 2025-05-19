// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponUnAttachNotify.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Weapon/BaseWeapon.h"

void UWeaponUnAttachNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	ACombatMechCharacter* Character = Cast<ACombatMechCharacter>(MeshComp->GetOwner());
	if (!Character) return;
	
	switch (Character->GetEquippedWeaponIndex())
	{
	case 1:
		if (!Character->SpawnedKatana) return;
		Character->SpawnedKatana->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Character->KatanaHolsterSocket);
		break;
	}

	Character->bIsEquipped = false;
	Character->EquippedWeaponIndex = -1;
}
