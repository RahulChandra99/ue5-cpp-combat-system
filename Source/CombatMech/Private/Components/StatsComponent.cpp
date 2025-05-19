// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatsComponent.h"

#include "CombatMech/CombatMechCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Kismet/KismetSystemLibrary.h"

UStatsComponent::UStatsComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatCharacterRef = Cast<ACombatMechCharacter>(GetOwner());
	
}


void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//RegenStamina();

	if(bIsSelfHealing)
	{
		RegenStamina();
		RegenHealth();
	}
		
}

void UStatsComponent::ReduceHealth(float Amount)
{
	if (Stats[EStat::Health] <= 0) return;

	Stats[EStat::Health] -= Amount;
	Stats[EStat::Health] = UKismetMathLibrary::Clamp(Stats[EStat::Health],0,Stats[EStat::MaxHealth]);
	
	OnHealthPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Health, EStat::MaxHealth));
}

void UStatsComponent::ReduceStamina(float Amount)
{
	if(Stats[EStat::Stamina] <= 0) return;
	
	Stats[EStat::Stamina] -= Amount;
	Stats[EStat::Stamina] = UKismetMathLibrary::Clamp(Stats[EStat::Stamina], 0, Stats[EStat::MaxStamina]);

	/*FLatentActionInfo FunctionInfo{
		0,
		100,
		TEXT("EnableRegeneration"),
		this
	};
	
	UKismetSystemLibrary::RetriggerableDelay(
		GetWorld(),
		StaminaDelayDuration,
		FunctionInfo
	);*/

	OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina,EStat::MaxStamina));
}





void UStatsComponent::EnableRegeneration()
{
	bCanRegen = true;
}

float UStatsComponent::GetStatPercentage(EStat Current, EStat Max)
{
	return Stats[Current]/Stats[Max];
}


void UStatsComponent::DrinkPotion(int PotionValue)
{
	
	switch (PotionValue)
	{
	case 0:
		if(bIsSelfHealing) return;
		//Health Potion
		bIsSelfHealing = true;
		
		if(CombatCharacterRef && HealthAnimMontage != nullptr)
			CombatCharacterRef->PlayAnimMontage(HealthAnimMontage);

		UGameplayStatics::PlaySoundAtLocation(this, HealthPotionSFX, CombatCharacterRef->GetActorLocation());
		if (HealthPotionVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				HealthPotionVFX,
				CombatCharacterRef->GetMesh(),
				HealthVFXSpawnLocation,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true
			);
		}

	break;
	}
}

void UStatsComponent::RegenHealth()
{
	if(!bIsSelfHealing) return;
	UWorld* World = GetWorld();
	if (!World) return;
	
	float DeltaTime = World->DeltaTimeSeconds;

	Stats[EStat::Health] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStat::Health],
		Stats[EStat::MaxHealth],
		DeltaTime,
		HealthRegenRate);

	OnHealthPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Health, EStat::MaxHealth));
	
}

void UStatsComponent::HandleResetPotion()
{
	bIsSelfHealing = false;
}

bool UStatsComponent::HasEnoughStamina()
{
	return Stats[Stamina] > 0;
}


void UStatsComponent::RegenStamina()
{
	if(!bIsSelfHealing) return;
	
	UWorld* World = GetWorld();
	if (!World) return;

	float DeltaTime = World->DeltaTimeSeconds;

	Stats[EStat::Stamina] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[EStat::Stamina],
		Stats[EStat::MaxStamina],
		DeltaTime,
		StaminaRegenRate);

	OnStaminaPercentUpdateDelegate.Broadcast(GetStatPercentage(EStat::Stamina, EStat::MaxStamina));
}
