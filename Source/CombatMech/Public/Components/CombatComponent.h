// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class UTraceComponent;
class UStatsComponent;
class UPlayerActionsComponent;
class ACombatMechCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATMECH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void CombatAttack(int AttackValue);

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	FORCEINLINE bool GetIsDefending() const{return bIsDefending;}

	void ToggleBlockAttack();


	UPROPERTY(VisibleAnywhere, Category = "AttackAnimations")
	int LightAttackComboCounter = 0;

	UPROPERTY(VisibleAnywhere, Category = "AttackAnimations")
	int HeavyAttackComboCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool bIsLightAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool bIsHeavyAttacking;
		
	
protected:
	virtual void BeginPlay() override;

	ACombatMechCharacter* CombatCharacterRef;
	UPlayerActionsComponent* PlayerActionsComponentRef;
	

	UPROPERTY(EditAnywhere, Category="AttackAnimations|Sword")
	TArray<UAnimMontage*> LightAttackSwordAnimationMontages;

	UPROPERTY(EditAnywhere, Category="AttackAnimations|Sword")
	TArray<UAnimMontage*> HeavyAttackSwordAnimationMontages;

	UPROPERTY(EditAnywhere, Category="AttackAnimations|DualKatana")
	TArray<UAnimMontage*> LightAttackDualKatanaAnimationMontages;

	UPROPERTY(EditAnywhere, Category="AttackAnimations|DualKatana")
	TArray<UAnimMontage*> HeavyAttackDualKatanaAnimationMontages;

	UPROPERTY(EditAnywhere, Category="AttackAnimations|AxeShield")
	TArray<UAnimMontage*> LightAttackAxeShieldAnimationMontages;

	UPROPERTY(EditAnywhere, Category="AttackAnimations|AxeShield")
	TArray<UAnimMontage*> HeavyAttackAxeShieldAnimationMontages;

	UPROPERTY(EditAnywhere, Category="AttackAnimations")
	TArray<TSubclassOf<UCameraShakeBase>> CameraShakes;

	

	FTimerHandle ComboTimer;
	FTimerHandle ComboResetTimer;
	
	void ResetCombo();

	bool bIsComboStarted;

	UPROPERTY(VisibleAnywhere,Category="AttackAnimations")
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category="Combat")
	bool bIsDefending;

	
	UStatsComponent* StatsComponentRef;
	UTraceComponent* TraceComponentRef;
	
		
};
