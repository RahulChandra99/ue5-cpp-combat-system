// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponAttachNotify.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Weapon/BaseWeapon.h"

void UWeaponAttachNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	ACombatMechCharacter* Character = Cast<ACombatMechCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	if(!Character->SpawnedKatana || !Character->SpawnedDualLeft || !Character->SpawnedDualRight || !Character->SpawnedAxe || !Character->SpawnedShield) return;
	
	switch (Character->GetEquippedWeaponIndex())
	{
	case 1:
		Character->SpawnedKatana->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Character->KatanaHandSocket);
		break;
	case 2:
		Character->SpawnedDualLeft->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Character->DualLeftHandSocket);
		Character->SpawnedDualRight->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Character->DualRightHandSocket);
		break;
	case 3:
		Character->SpawnedShield->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Character->ShieldHandSocket);
		Character->SpawnedAxe->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, Character->AxeHandSocket);
		break;
	}
	
	
	
}
