// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/BaseWeapon.h"
#include "TraceComponent.generated.h"


class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FHitEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* ImpactSound;
};


class ACombatMechCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATMECH_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTraceComponent();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* SwordSwingSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UNiagaraSystem* TrailEffectNiagara;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> BloodSplatterActor;
	
	UPROPERTY()
	class UNiagaraComponent* TrailEffectNiagaraComp = nullptr; // Class member

	
	UFUNCTION()
	void PlayTrailAndSFX(int WeaponIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TMap<TEnumAsByte<EPhysicalSurface>, FHitEffectData> HitEffectMap;

	;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PerformWeaponTrace();

	ACombatMechCharacter* CombatCharacterRef;
	int CurrentWeaponEquippedIndex;
	ABaseWeapon* CurrentEquippedWeapon;
	
	UPROPERTY(EditAnywhere)
	bool bDebugMode = false;


	TArray<AActor*> TargetsToIgnore;

	

		
};
