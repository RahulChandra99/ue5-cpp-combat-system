// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BossCharacter.h"

#include "Components/StatsComponent.h"

ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StatsComponent = CreateDefaultSubobject<UStatsComponent>("StatsComponent");

}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{
	if (DetectedPawn != PawnToDetect)
	{
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player was detected"));
	}
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

