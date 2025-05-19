// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerActionsComponent.generated.h"

class ULockOnComponent;
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnSprintSignature,
	UPlayerActionsComponent, OnSprintDelegate,
	float, Cost
);

UENUM(BlueprintType)
enum class ERollDirection : uint8
{
	Forward,
	ForwardRight,
	Right,
	BackwardRight,
	Backward,
	BackwardLeft,
	Left,
	ForwardLeft
};

class IMainPlayer;
class UCharacterMovementComponent;
class ACombatMechCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATMECH_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerActionsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ResetRoll();

protected:
	virtual void BeginPlay() override;

	ACombatMechCharacter* CombatCharacterRef;
	UCharacterMovementComponent* CharacterMovementComponent;
	IMainPlayer* IPlayerRef;

	UPROPERTY(EditAnywhere)
	float SprintCost = 0.1f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 125.f;

	ERollDirection GetRollDirection();

	ULockOnComponent* LockOnComponentRef;


public:
	

	UPROPERTY(BlueprintAssignable)
	FOnSprintSignature OnSprintDelegate;

	UFUNCTION(BlueprintCallable)
	void Crouch();

	FORCEINLINE bool GetIsSprinting() const{return bIsSprinting;}
	FORCEINLINE bool GetIsCrouching() const{return bIsCrouching;}
	FORCEINLINE bool GetIsRolling() const{return bIsRolling;}

	void LaunchRoll();

	UFUNCTION()
	void OnSprintRollPressed();

	UFUNCTION()
	void OnSprintRollReleased();

	void StartSprint();
	void StopSprint();
	void PerformRoll();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category="Combat")
	bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category="Combat")
	bool bIsCrouching;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category="Combat")
	bool bIsRolling;

	UPROPERTY(EditAnywhere, Category="Combat|Katana")
	class UAnimMontage* KatanaEquipAM;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Katana")
	TMap<ERollDirection, UAnimMontage*> DodgeMontages;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Katana")
	float RollStrength = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Combat|Katana")
	float RollDuration = 0.6f;

	FTimerHandle DodgeTimerHandle;

	void Roll();

	FTimerHandle SprintRollTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	float ShiftHoldThreshold = 0.4f;
	
	bool bWantsToSprint = false;

	
	

	
		
};
