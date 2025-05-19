// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EStat.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

class ACombatMechCharacter;
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnHealthPercentUpdateSignature,
	UStatsComponent, OnHealthPercentUpdateDelegate,
	float, Percentage);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnStaminaPercentUpdateSignature,
	UStatsComponent, OnStaminaPercentUpdateDelegate,
	float, Percentage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATMECH_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	double StaminaRegenRate = 10.f;

	UPROPERTY(VisibleAnywhere)
	bool bCanRegen = true;

	UPROPERTY(EditAnywhere)
	float StaminaDelayDuration = 2.f;

	ACombatMechCharacter* CombatCharacterRef;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Potion|Health")
	UAnimMontage* HealthAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Potion|Health")
	USoundBase* HealthPotionSFX;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Potion|Health")
	UNiagaraSystem* HealthPotionVFX;

	UPROPERTY(EditDefaultsOnly,Category="Combat|Potion|Health")
	double HealthRegenRate = 10.f;

	UPROPERTY(EditDefaultsOnly,Category="Combat|Potion|Health")
	FName HealthVFXSpawnLocation;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount);

	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();

	UFUNCTION(BlueprintCallable)
	void RegenHealth();

	UFUNCTION()
	void EnableRegeneration();

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>, float> Stats;

	UFUNCTION(BlueprintPure)
	float GetStatPercentage(EStat Current, EStat Max);

	UPROPERTY(BlueprintAssignable)
	FOnHealthPercentUpdateSignature OnHealthPercentUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaPercentUpdateSignature OnStaminaPercentUpdateDelegate;

	UFUNCTION()
	void DrinkPotion(int PotionValue);

	UFUNCTION(BlueprintCallable)
	void HandleResetPotion();

	UPROPERTY(VisibleAnywhere)
	bool bIsSelfHealing;

	UFUNCTION(BlueprintImplementableEvent)
	void HealthPotionVisual();

	UFUNCTION()
	bool HasEnoughStamina();
	
};
