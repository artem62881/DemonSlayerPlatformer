// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/PlayerHUDWidget.h"
#include "DSPlayerController.generated.h"

/**
 * 
 */
class ADSPlayerCharacter;

UCLASS()
class DEMONSLAYER_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetPawn(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPlayerHUDWidget> PlayerHUDWidgetClass;

private:
	TSoftObjectPtr<ADSPlayerCharacter> CachedPlayer;
	class UPlayerHUDWidget* PlayerHUDWidget = nullptr;
		
	void Jump();
	void StopJumping();
	void ChangeCrouchState();
	void MoveRight(float Value);

	void Attack();
	void ChangeDownAttackMod();

	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	void CreateAndInitWidgets();
};
