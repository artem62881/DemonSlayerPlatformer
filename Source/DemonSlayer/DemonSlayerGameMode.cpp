// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemonSlayerGameMode.h"
#include "Characters/DSPlayerCharacter.h"

ADemonSlayerGameMode::ADemonSlayerGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ADSPlayerCharacter::StaticClass();
}
