// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyAnimInstance.h"

#include "Character/BossCharacter.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EnemyCharacterRef = Cast<ABossCharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if(EnemyCharacterRef == nullptr)
	{
		EnemyCharacterRef = Cast<ABossCharacter>(TryGetPawnOwner());
	}

	if(EnemyCharacterRef == nullptr) return;

	FVector Velocity = EnemyCharacterRef->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();


	FRotator ActorRotation = EnemyCharacterRef->GetActorRotation();
	Direction = CalculateDirection(Velocity,ActorRotation);
	
}