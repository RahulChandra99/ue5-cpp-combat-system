// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RollLaunchNotify.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Components/PlayerActionsComponent.h"


void URollLaunchNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	if (!MeshComp) return;

	ACombatMechCharacter* Character = Cast<ACombatMechCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	Character->PlayerActionsComponent->LaunchRoll();
	
}
