// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DemonSlayerCharacter.generated.h"

class UTextRenderComponent;
class UPaperFlipbook;
/**
 * This class is the default character for DemonSlayer, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

USTRUCT(BlueprintType)
struct FBasicAttackType
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//FName AttackName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDisableMovement = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Direction = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Impulse = 40.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 20.f;
};

UCLASS(config=Game)
class ADemonSlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
	UTextRenderComponent* TextComponent;
	
protected:
	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* IdleAnimation;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	UPaperFlipbook* RunningAnimation;
	
	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* FallAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* LandAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* DamagedAnimation;
	
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	virtual void OnJumped_Implementation() override;
	
	void UpdateCharacter();
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bDrawDebug = false;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes, meta = (UIMin = 0.f, ClampMin = 0.f))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (UIMin = 0.f, ClampMin = 0.f))
	float DestroyTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (UIMin = 0.f, ClampMin = 0.f))
	uint8 MaxAirAttackCount = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float HardLandingHeight = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	UCurveFloat* FallDamageCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DefaultFallDamage = 10.f;
	
public:
	ADemonSlayerCharacter();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void NotifyJumpApex() override;
	
	void PlayAnimation(UPaperFlipbook* AnimMontage, bool bDisableMove = false);

	UFUNCTION(BlueprintCallable)
	void PerformLinearAttack(const FBasicAttackType AttackType);
	
	virtual void Falling() override;

	UFUNCTION(BlueprintNativeEvent)
	void Attack();
	virtual void Attack_Implementation();

	UPROPERTY(BlueprintAssignable)
	FOnDeathSignature OnDeath;

	UFUNCTION()
	virtual void OnLandedEvent(const FHitResult& Hit);

	UFUNCTION()
	virtual void OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void OnDeathEvent();
	
	UFUNCTION(BlueprintCallable)
	bool IsAttacking() const { return bIsAttacking; };
	
private:
	bool bIsAnimMontagePlaying = false;

	bool bIsDead = false;

	bool bIsAttacking = false;

	uint8 AirAttackCount;

	float Health = 0.f;

	float CurrentApex = 0.f;

	FTimerHandle DestroyTimer;

	void DestroyActor();
	
	UFUNCTION()
	void OnAnimMontageEnded();
};