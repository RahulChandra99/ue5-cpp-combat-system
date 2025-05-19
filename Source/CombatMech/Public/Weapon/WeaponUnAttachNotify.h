// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "WeaponUnAttachNotify.generated.h"

/**
 * 
 */
UCLASS()
class COMBATMECH_API UWeaponUnAttachNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:

    void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
