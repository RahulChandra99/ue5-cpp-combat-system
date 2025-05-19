// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SwordTrailNotify.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Components/TraceComponent.h"

void USwordTrailNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceComponent = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();
	if(!IsValid(TraceComponent)) return;

	if (!MeshComp) return;

	ACombatMechCharacter* Character = Cast<ACombatMechCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	TraceComponent->PlayTrailAndSFX(Character->GetEquippedWeaponIndex());
}
