// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Components/PlayerActionsComponent.h"
#include "Components/StatsComponent.h"
#include "Components/TraceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatCharacterRef = Cast<ACombatMechCharacter>(GetOwner());
	if(CombatCharacterRef)
	{
		PlayerActionsComponentRef = CombatCharacterRef->PlayerActionsComponent;
		StatsComponentRef = CombatCharacterRef->StatsComponent;
		TraceComponentRef = CombatCharacterRef->TraceComponent;
	}
		
}




// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCombatComponent::ToggleBlockAttack()
{
	bIsDefending = !bIsDefending;

	if(bIsDefending)
	{
		if (PlayerActionsComponentRef->GetIsSprinting())
		{
			// Cancel sprinting if trying to defend
			PlayerActionsComponentRef->StopSprint();
		}
	}
	
}


void UCombatComponent::CombatAttack(int AttackValue)
{
	if (!bCanAttack || !CombatCharacterRef || StatsComponentRef->bIsSelfHealing || !StatsComponentRef->HasEnoughStamina()) return;

	// Example: AttackValue 0 = Light attack
	if (AttackValue == 0 && CombatCharacterRef->GetEquippedWeaponIndex() == 1)
	{
		bIsLightAttacking = true;
		bIsComboStarted = true;
		bCanAttack = false;

		// Play combo animation
		if (LightAttackSwordAnimationMontages.IsValidIndex(LightAttackComboCounter))
		{
			CombatCharacterRef->PlayAnimMontage(LightAttackSwordAnimationMontages[LightAttackComboCounter]);
		}
		

		// Increase combo step (wrap to 0 if needed)
		LightAttackComboCounter = (LightAttackComboCounter + 1) % LightAttackSwordAnimationMontages.Num();
		

		if(StatsComponentRef)
			StatsComponentRef->ReduceStamina(10);
	}
	else if (AttackValue == 1 && CombatCharacterRef->GetEquippedWeaponIndex() == 1)
	{
		bIsHeavyAttacking = true;
		bIsComboStarted = true;
		bCanAttack = false;

		// Play combo animation
		if (HeavyAttackSwordAnimationMontages.IsValidIndex(HeavyAttackComboCounter))
		{
			CombatCharacterRef->PlayAnimMontage(HeavyAttackSwordAnimationMontages[HeavyAttackComboCounter]);
		}
		

		// Increase combo step (wrap to 0 if needed)
		HeavyAttackComboCounter = (HeavyAttackComboCounter + 1) % HeavyAttackSwordAnimationMontages.Num();

		if(StatsComponentRef)
			StatsComponentRef->ReduceStamina(20);
	}
}


void UCombatComponent::HandleResetAttack()
{
	bCanAttack = true;

	// Start 1.5s timer to reset combo if player doesn't press again
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimer);
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimer, this, &UCombatComponent::ResetCombo, 1.1f, false);
}



void UCombatComponent::ResetCombo()
{
	if (LightAttackComboCounter>0)
	{
		LightAttackComboCounter = 0;
		bIsLightAttacking = false;
	}
		
	else if (HeavyAttackComboCounter > 0)
	{
		HeavyAttackComboCounter = 0;
		bIsHeavyAttacking = false;
	}
		
	
	bIsComboStarted = false;

	UE_LOG(LogTemp, Warning, TEXT("Combo reset due to timeout."));
}


