

#include "Components/TraceComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "CombatMech/CombatMechCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Kismet/KismetSystemLibrary.h"


UTraceComponent::UTraceComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UTraceComponent::HandleResetAttack()
{
	if(!TargetsToIgnore.IsEmpty())
		TargetsToIgnore.Empty();

	
}


void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CombatCharacterRef = Cast<ACombatMechCharacter>(GetOwner());
	
}



void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking || !CombatCharacterRef) return;

	CurrentWeaponEquippedIndex = CombatCharacterRef->GetEquippedWeaponIndex();

	if (CurrentWeaponEquippedIndex == 1)
	{
		PerformWeaponTrace();
	}
}

void UTraceComponent::PerformWeaponTrace()
{
	CurrentEquippedWeapon = CombatCharacterRef->SpawnedKatana;
	if (!CurrentEquippedWeapon) return;

	FVector StartSocketLocation = CurrentEquippedWeapon->WeaponMesh->GetSocketLocation(CurrentEquippedWeapon->StartTraceSocketName);
	FVector EndSocketLocation = CurrentEquippedWeapon->WeaponMesh->GetSocketLocation(CurrentEquippedWeapon->EndTraceSocketName);
	FQuat ShapeRotation = CurrentEquippedWeapon->WeaponMesh->GetSocketQuaternion(CurrentEquippedWeapon->Rotation);

	double DistanceBWWeaponSockets = FVector::Distance(StartSocketLocation, EndSocketLocation);
	FVector BoxHalfExtent {30.f, 30.f, DistanceBWWeaponSockets / 2.0};
	FCollisionShape Box = FCollisionShape::MakeBox(BoxHalfExtent);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams IgnoreParams(FName(TEXT("Ignore Params")), false, GetOwner());
	IgnoreParams.bReturnPhysicalMaterial = true;

	bool bHasFoundTargets = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartSocketLocation,
		EndSocketLocation,
		ShapeRotation,
		ECC_GameTraceChannel1,
		Box,
		IgnoreParams
	);
	

	if (HitResults.Num() == 0) return;

	float CharacterDamage = 0.f;
	if (IFighter* FighterRef = Cast<IFighter>(GetOwner()))
	{
		CharacterDamage = FighterRef->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* TargetActor = Hit.GetActor();
		if (!TargetActor || TargetsToIgnore.Contains(TargetActor)) continue;

		// Surface effect logic
		if (Hit.PhysMaterial.IsValid())
		{
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			if (HitEffectMap.Contains(SurfaceType))
			{
				const FHitEffectData& EffectData = HitEffectMap[SurfaceType];

				if (EffectData.ImpactEffect)
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					EffectData.ImpactEffect,
					Hit.ImpactPoint,
					Hit.ImpactNormal.Rotation());

				}

				if (EffectData.ImpactSound)
				{
					UGameplayStatics::PlaySoundAtLocation(
						GetWorld(),
						EffectData.ImpactSound,
						Hit.ImpactPoint
					);
				}

				if (CombatCharacterRef && CombatCharacterRef->SpawnedKatana)
				{

					if (BloodSplatterActor)
					{
						FRotator SplatterRotation = FRotator::ZeroRotator; 

						FActorSpawnParameters SpawnParams;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						GetWorld()->SpawnActor<AActor>(
							BloodSplatterActor,
							Hit.ImpactPoint,
							SplatterRotation,
							SpawnParams
						);
					}
				}
				CombatCharacterRef->CameraShakeVisual();
				
			}
		}

		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);

		TargetsToIgnore.AddUnique(TargetActor);
	}
}

void UTraceComponent::PlayTrailAndSFX(int WeaponIndex)
{
	switch (WeaponIndex)
	{
	case 1:
		// Play swing sound
			UGameplayStatics::PlaySound2D(GetWorld(), SwordSwingSound);

		// Play sword trail Niagara effect
		if (CombatCharacterRef && CombatCharacterRef->SpawnedKatana && TrailEffectNiagara)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				TrailEffectNiagara, // UNiagaraSystem*
				CombatCharacterRef->SpawnedKatana->WeaponMesh, // Parent component
				CombatCharacterRef->SpawnedKatana->EndTraceSocketName, // Socket name (e.g. "Blade_Tip")
				FVector::ZeroVector, // Location offset
				FRotator::ZeroRotator, // Rotation offset
				EAttachLocation::SnapToTargetIncludingScale, // Attach mode
				true, // Auto destroy
				true // Auto activate
			);
		}


		





		break;
	}
}


