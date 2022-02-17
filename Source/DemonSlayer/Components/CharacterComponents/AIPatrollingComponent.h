// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Navigation/PatrollingPath.h"
#include "Components/ActorComponent.h"
#include "AIPatrollingComponent.generated.h"


class APatrollingPath;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEMONSLAYER_API UAIPatrollingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	bool CanPatrol() const;

	FVector GetClosestWayPoint();
	FVector GetNextWayPoint();
	
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Path")
	APatrollingPath* PatrollingPath;

private:
	int32 CurrentWayPointIndex = -1;

};
