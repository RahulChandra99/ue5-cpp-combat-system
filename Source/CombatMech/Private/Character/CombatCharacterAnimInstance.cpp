// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMech/Public/Character/CombatCharacterAnimInstance.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/PlayerActionsComponent.h"
#include "Components/StatsComponent.h"


void UCombatCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CombatCharacterRef = Cast<ACombatMechCharacter>(TryGetPawnOwner());
}

void UCombatCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if(CombatCharacterRef == nullptr)
	{
		CombatCharacterRef = Cast<ACombatMechCharacter>(TryGetPawnOwner());
	}

	if(CombatCharacterRef == nullptr) return;

	FVector Velocity = CombatCharacterRef->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();


	FRotator ActorRotation = CombatCharacterRef->GetActorRotation();
	Direction = CalculateDirection(Velocity,ActorRotation);

	bEquipped = CombatCharacterRef->GetIsEquipped();
	bDefense = CombatCharacterRef->CombatComponent->GetIsDefending();
	bCrouched = CombatCharacterRef->PlayerActionsComponent->GetIsCrouching();
	bDrinkingPotion = CombatCharacterRef->StatsComponent->bIsSelfHealing;

	EquippedWeaponIndex = CombatCharacterRef->GetEquippedWeaponIndex();
	
	
}
