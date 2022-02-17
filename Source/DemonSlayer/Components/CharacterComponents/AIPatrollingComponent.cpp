// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPatrollingComponent.h"

bool UAIPatrollingComponent::CanPatrol() const
{
	return IsValid(PatrollingPath) && PatrollingPath->GetWayPoints().Num() > 0;
}

FVector UAIPatrollingComponent::GetClosestWayPoint()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	
	if (!CanPatrol())
	{
		return OwnerLocation;
	}
	
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	FTransform PathTransform = PatrollingPath->GetTransform();

	FVector ClosestWayPoint;
	float MinSqDist = FLT_MAX;
	
	for (int32 i = 0; i < WayPoints.Num(); ++i)
	{
		FVector WayPoint = PathTransform.TransformPosition(WayPoints[i]);
		float CurrentSqDist = FVector::DistSquared(OwnerLocation, WayPoint);
		
		if (CurrentSqDist < MinSqDist)
		{
			CurrentWayPointIndex = i;
			ClosestWayPoint = WayPoint;
			MinSqDist = CurrentSqDist;
		}
	}
	
	return ClosestWayPoint;
}

FVector UAIPatrollingComponent::GetNextWayPoint()
{
	++CurrentWayPointIndex;
	
	const TArray<FVector> WayPoints = PatrollingPath->GetWayPoints();
	if (!WayPoints.IsValidIndex(CurrentWayPointIndex))
	{
		CurrentWayPointIndex = 0;	
	}

	FTransform PathTransform = PatrollingPath->GetActorTransform();
	FVector NextWayPoint = PathTransform.TransformPosition(WayPoints[CurrentWayPointIndex]);
	
	return NextWayPoint;
}
