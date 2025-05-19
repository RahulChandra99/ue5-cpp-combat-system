// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PlayerActionsComponent.h"

#include "Camera/CameraComponent.h"
#include "CombatMech/CombatMechCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/LockOnComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/MainPlayer.h"

UPlayerActionsComponent::UPlayerActionsComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatCharacterRef = Cast<ACombatMechCharacter>(GetOwner());
	CharacterMovementComponent = CombatCharacterRef->GetCharacterMovement();
	if(CombatCharacterRef->Implements<UMainPlayer>())
	{
		IPlayerRef = Cast<IMainPlayer>(CombatCharacterRef);
	}

	LockOnComponentRef = CombatCharacterRef->FindComponentByClass<ULockOnComponent>();
	
}


void UPlayerActionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



void UPlayerActionsComponent::Crouch()
{
	bIsCrouching = !bIsCrouching;

	if(bIsCrouching)
	{
		CharacterMovementComponent->Crouch();
		
		if(bIsSprinting)
			CharacterMovementComponent->MaxWalkSpeedCrouched = SprintSpeed;
		else
			CharacterMovementComponent->MaxWalkSpeedCrouched = WalkSpeed;
	}
	else
	{
		CharacterMovementComponent->UnCrouch();
	}

	CombatCharacterRef->CrouchVisual();
}



void UPlayerActionsComponent::OnSprintRollPressed()
{

	GetWorld()->GetTimerManager().SetTimer(SprintRollTimerHandle, this, &UPlayerActionsComponent::StartSprint,ShiftHoldThreshold, false);
}

void UPlayerActionsComponent::OnSprintRollReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(SprintRollTimerHandle);
	
	if(!bWantsToSprint)
	{
		PerformRoll();
	}
	else
	{
		StopSprint();
	}
	

}

void UPlayerActionsComponent::StartSprint()
{
	bWantsToSprint = true;
	if(CharacterMovementComponent->Velocity.Equals(FVector::ZeroVector,1)) return;

	if (CombatCharacterRef->CombatComponent->GetIsDefending())
	{
		// Cancel defending if trying to sprint
		//bIsDefending = false;
	}

	bIsSprinting = true;
	
	CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;

	CombatCharacterRef->SprintVisual();
}

void UPlayerActionsComponent::StopSprint()
{
	bWantsToSprint = false;
	bIsSprinting = false;
	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;

	CombatCharacterRef->SprintVisual();
}

void UPlayerActionsComponent::PerformRoll()
{
	Roll();
}


void UPlayerActionsComponent::Roll()
{
	if (bIsRolling || CharacterMovementComponent->IsFalling() || CharacterMovementComponent->Velocity.Equals(FVector::ZeroVector,1)) return;

	UE_LOG(LogTemp, Warning, TEXT("Rolling 2"));

	bIsRolling = true;

	ERollDirection Dir = GetRollDirection();

	UAnimMontage* Montage = DodgeMontages.Contains(Dir) ? DodgeMontages[Dir] : nullptr;
	if (Montage)
	{
		CombatCharacterRef->PlayAnimMontage(Montage);
	}
}

ERollDirection UPlayerActionsComponent::GetRollDirection()
{
	FVector Input = CombatCharacterRef->GetLastMovementInputVector();
	if (Input.IsNearlyZero())
		return ERollDirection::Forward;

	Input.Normalize();

	FVector CameraForward = CombatCharacterRef->GetFollowCamera()->GetForwardVector();
	FVector CameraRight = CombatCharacterRef->GetFollowCamera()->GetRightVector();

	CameraForward.Z = 0;
	CameraRight.Z = 0;
	CameraForward.Normalize();
	CameraRight.Normalize();

	// Convert input to camera-relative space
	float ForwardDot = FVector::DotProduct(CameraForward, Input);
	float RightDot = FVector::DotProduct(CameraRight, Input);

	float Angle = FMath::RadiansToDegrees(FMath::Atan2(RightDot, ForwardDot));
	if (Angle < 0) Angle += 360;

	if(!LockOnComponentRef->GetIsLockedOn()) return ERollDirection::Forward;
	
	if (Angle < 22.5f || Angle >= 337.5f) return ERollDirection::Forward;
	if (Angle < 67.5f) return ERollDirection::ForwardRight;
	if (Angle < 112.5f) return ERollDirection::Right;
	if (Angle < 157.5f) return ERollDirection::BackwardRight;
	if (Angle < 202.5f) return ERollDirection::Backward;
	if (Angle < 247.5f) return ERollDirection::BackwardLeft;
	if (Angle < 292.5f) return ERollDirection::Left;
	return ERollDirection::ForwardLeft;
}

void UPlayerActionsComponent::LaunchRoll()
{
	FVector InputDir = CombatCharacterRef->GetLastMovementInputVector();
	if (InputDir.IsNearlyZero())
		InputDir = CombatCharacterRef->GetActorForwardVector();

	FVector LaunchDir = InputDir.GetSafeNormal();
	CombatCharacterRef->LaunchCharacter(LaunchDir * RollStrength, true, true);

	CombatCharacterRef->GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &UPlayerActionsComponent::ResetRoll, RollDuration, false);
}

void UPlayerActionsComponent::ResetRoll()
{
	bIsRolling = false;
	
}




