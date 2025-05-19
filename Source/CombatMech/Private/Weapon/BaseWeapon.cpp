// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh = FindComponentByClass<UStaticMeshComponent>();
	
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

