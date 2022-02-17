// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagingActor.h"
#include "PaperFlipbookComponent.h"
#include "Characters/DemonSlayerCharacter.h"

// Sets default values
ADamagingActor::ADamagingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	DamageBox->SetupAttachment(RootComponent);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(DamageBox);
}

// Called when the game starts or when spawned
void ADamagingActor::BeginPlay()
{
	Super::BeginPlay();
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ADamagingActor::OnBoxBeginOverlap);
	DamageBox->OnComponentEndOverlap.AddDynamic(this, &ADamagingActor::OnBoxEndOverlap);
}

void ADamagingActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("ADamagingActor::OnBoxBeginOverlap 1"));
	if (!OtherActor->IsA<APawn>())
	{
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("ADamagingActor::OnBoxBeginOverlap 2"));
	DamageActor(OtherActor);
	
	if (bIsDamageSequenced)
	{
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &ADamagingActor::DamageActor, OtherActor);
		GetWorld()->GetTimerManager().SetTimer(DamageTimer, TimerDel, DamageIntervals, true);
	}
}

void ADamagingActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsDamageSequenced)
	{
		GetWorld()->GetTimerManager().ClearTimer(DamageTimer);
	}
}

void ADamagingActor::DamageActor(AActor* OtherActor)
{
	UE_LOG(LogTemp, Error, TEXT("ADamagingActor::DamageActor"));
	OtherActor->TakeDamage(DamageAmount, FDamageEvent{}, NULL, this);
}

// Called every frame
void ADamagingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

