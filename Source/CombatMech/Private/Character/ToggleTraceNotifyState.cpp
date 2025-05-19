// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ToggleTraceNotifyState.h"

#include "Components/TraceComponent.h"


void UToggleTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceComponent = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();
	if(!IsValid(TraceComponent)) return;

	TraceComponent->bIsAttacking = true;

	
}

void UToggleTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UTraceComponent* TraceComponent = MeshComp->GetOwner()->FindComponentByClass<UTraceComponent>();
	if(!IsValid(TraceComponent)) return;

	TraceComponent->bIsAttacking = false;
}

