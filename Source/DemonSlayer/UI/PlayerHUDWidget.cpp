// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "AttributesWidget.h"
#include "Blueprint/WidgetTree.h"

UUserWidget* UPlayerHUDWidget::GetAttributesWidget() const
{
	return WidgetTree->FindWidget<UAttributesWidget>(AttributesWidgetName);
}
