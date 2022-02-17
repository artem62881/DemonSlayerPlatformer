// Fill out your copyright notice in the Description page of Project Settings.


#include "DSAICharacter.h"

ADSAICharacter::ADSAICharacter()
{
	PatrollingComponent = CreateDefaultSubobject<UAIPatrollingComponent>(TEXT("AIPatrollingComponent"));
}
