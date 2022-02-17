// Fill out your copyright notice in the Description page of Project Settings.


#include "DSAIController.h"
#include "Characters/DSAICharacter.h"
#include "Perception/AIPerceptionComponent.h"

ADSAIController::ADSAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void ADSAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		checkf(InPawn->IsA<ADSAICharacter>(), TEXT("ADSAIController::SetPawn(APawn* InPawn) | DSAIController can be used only with DSAICharacter"));
		CachedAICharacter = Cast<ADSAICharacter>(InPawn);
		RunBehaviorTree(CachedAICharacter->GetBehaviorTree());
	}
	else
	{
		CachedAICharacter = NULL;
	}
}

void ADSAIController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);
}

void ADSAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
}

void ADSAIController::BeginPlay()
{
	Super::BeginPlay();
}
