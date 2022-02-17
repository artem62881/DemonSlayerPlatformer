// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemonSlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../Components/CharacterComponents/AIPatrollingComponent.h"
#include "DSAICharacter.generated.h"

/**
 * 
 */
class UBehaviorTree;
class UAIPatrollingComponent;

UCLASS()
class DEMONSLAYER_API ADSAICharacter : public ADemonSlayerCharacter
{
	GENERATED_BODY()

	ADSAICharacter();

public:
	
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; };
	UAIPatrollingComponent* GetPatrollingComponent() const { return PatrollingComponent; };
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAIPatrollingComponent* PatrollingComponent; 
	
};
