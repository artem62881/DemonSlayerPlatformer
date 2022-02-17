// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DamagingActor.generated.h"

UCLASS()
class DEMONSLAYER_API ADamagingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagingActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* DamageBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPaperFlipbookComponent* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageAmount = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDamageSequenced = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageIntervals = 1.f;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:	
	FTimerHandle DamageTimer;

	void DamageActor(AActor* OtherActor);
	
};