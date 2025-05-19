// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"


class UCharacterMovementComponent;
class ACombatMechCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBATMECH_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULockOnComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

protected:
	virtual void BeginPlay() override;
	
private:

	ACombatMechCharacter* CombatCharacterRef;
	APlayerController* PlayerController;
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, Category="Combat")
	bool bIsLockedIn;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float TargetRotationZValue = 125.f;

public:
	
	void StartLockOn(float Radius);
	void EndLockOn();
	void ToggleLockOn(float Radius);
	
	AActor* CurrentTargetActor;
	
	
	FORCEINLINE bool GetIsLockedOn() const { return bIsLockedIn;}
		
};
