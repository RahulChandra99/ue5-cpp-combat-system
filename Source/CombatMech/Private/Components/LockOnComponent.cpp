// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LockOnComponent.h"

#include "CombatMech/CombatMechCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	CombatCharacterRef = Cast<ACombatMechCharacter>(GetOwner());
	PlayerController = GetWorld()->GetFirstPlayerController();
	CharacterMovementComponent = CombatCharacterRef->GetCharacterMovement();
}


void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//setting controller rotation when locked on
	if(IsValid(CurrentTargetActor))
	{
		FVector CombatCharacterLocation = CombatCharacterRef->GetActorLocation();
		FVector TargetActorLocation = CurrentTargetActor->GetActorLocation();
		
		TargetActorLocation.Z -= TargetRotationZValue;
		FRotator LookAtRot  = UKismetMathLibrary::FindLookAtRotation(CombatCharacterLocation,TargetActorLocation);
		PlayerController->SetControlRotation(LookAtRot );

		// Break lock if target is too far
        const float LockOnBreakDistance = 1000.f;
        if (FVector::DistSquared(CombatCharacterLocation, TargetActorLocation) > FMath::Square(LockOnBreakDistance))
        {
        	EndLockOn();
        	return;
        }
	}

}

void ULockOnComponent::StartLockOn(float Radius)
{
	
	bIsLockedIn = true;
	if(CombatCharacterRef) CombatCharacterRef->LockInVisual(bIsLockedIn);
	FHitResult HitResult;
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams IgnoreParams{
		FName("Ignore Collision Params"), false,GetOwner()};

	bool bFoundTarget = GetWorld()->SweepSingleByChannel(
		HitResult,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere,
		IgnoreParams
	);

	if(bFoundTarget)
	{
		if(HitResult.GetActor()->Implements<UEnemy>())
		{
			CurrentTargetActor = HitResult.GetActor();
		
			PlayerController->SetIgnoreLookInput(true);
			CharacterMovementComponent->bOrientRotationToMovement = false;
			CharacterMovementComponent->bUseControllerDesiredRotation = true;
		
			CombatCharacterRef->GetCameraBoom()->TargetOffset = {0.f,0.f,30.f};
			IEnemy::Execute_OnLockedIn(CurrentTargetActor);
		}
		
		
	}
}

void ULockOnComponent::EndLockOn()
{
	bIsLockedIn = false;
	if(CombatCharacterRef) CombatCharacterRef->LockInVisual(bIsLockedIn);
	IEnemy::Execute_OnLockedOut(CurrentTargetActor);
	CurrentTargetActor = nullptr;
	
	PlayerController->ResetIgnoreLookInput();
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->bUseControllerDesiredRotation = false;
	
	CombatCharacterRef->GetCameraBoom()->TargetOffset = {0.f,0.f,0.f};
}

void ULockOnComponent::ToggleLockOn(float Radius)
{
	if(!IsValid(CurrentTargetActor))
	{
		StartLockOn(Radius);
	}
	else
	{
		EndLockOn();
	}
}
