// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"



UCLASS()
class COMBATMECH_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

protected:
	virtual void BeginPlay() override;

	

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	FName StartTraceSocketName;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FName EndTraceSocketName;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FName Rotation;

	UPROPERTY()
	UStaticMeshComponent* WeaponMesh;
};
