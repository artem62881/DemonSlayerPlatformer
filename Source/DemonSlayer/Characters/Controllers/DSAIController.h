// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DSAIController.generated.h"

/**
 * 
 */
UCLASS()
class DEMONSLAYER_API ADSAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADSAIController();
	
	virtual void SetPawn(APawn* InPawn) override;

	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	virtual  void BeginPlay() override;

private:
	TWeakObjectPtr<class ADSAICharacter> CachedAICharacter;
};
