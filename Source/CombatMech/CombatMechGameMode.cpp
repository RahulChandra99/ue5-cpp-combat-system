// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatMechGameMode.h"
#include "CombatMechCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACombatMechGameMode::ACombatMechGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
