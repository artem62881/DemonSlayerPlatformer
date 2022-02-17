// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesWidget.h"
#include "Characters/DemonSlayerCharacter.h"

float UAttributesWidget::GetHealthPercent() const
{
	float Result = 1.f;

	ADemonSlayerCharacter* OwningPlayer = Cast<ADemonSlayerCharacter>(GetOwningPlayerPawn());
	if (OwningPlayer)
	{
		Result = OwningPlayer->GetHealthPercent();
	}
	
	return Result;
}
