// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
class UUserWidget;

UCLASS()
class DEMONSLAYER_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UUserWidget* GetAttributesWidget() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category= "Widgets names")
	FName AttributesWidgetName = NAME_None;
};
