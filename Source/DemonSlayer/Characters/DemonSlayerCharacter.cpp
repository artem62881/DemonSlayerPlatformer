// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemonSlayerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ADemonSlayerCharacter

ADemonSlayerCharacter::ADemonSlayerCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
	
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

void ADemonSlayerCharacter::PlayAnimation(UPaperFlipbook* AnimMontage, bool bDisableMove /* = false*/)
{
	if (bIsDead)
	{
		return;
	}
	
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(AnimMontage);
	bIsAnimMontagePlaying = true;
	
	GetSprite()->PlayFromStart();
	if (bDisableMove)
	{
		GetCharacterMovement()->DisableMovement();
	}
	//UE_LOG(SideScrollerCharacter, Error, TEXT("AnimMontage | IsLooping: %i"), GetSprite()->IsLooping());
}

void ADemonSlayerCharacter::PerformLinearAttack(const FBasicAttackType AttackType)
{
	if (GetCharacterMovement()->IsFalling())
	{
		++AirAttackCount;
		if (AirAttackCount > MaxAirAttackCount)
		{
			return;
		}
	}

	float SphereRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	FVector TraceStart = GetActorLocation() + AttackType.Direction.GetSafeNormal() * SphereRadius;
	FVector TraceDirection = AttackType.Direction.GetSafeNormal();
	TraceDirection.X *= GetActorForwardVector().X;
	FVector TraceEnd = TraceStart + TraceDirection * AttackType.Distance;
	//FVector CapsuleExtent = GetCapsuleComponent()->GetCollisionShape().GetExtent();
	const FName CollisionProfile = UCollisionProfile::BlockAllDynamic_ProfileName;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
#if WITH_EDITORONLY_DATA
	DrawDebugType = bDrawDebug == true ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
#endif
	TArray<FHitResult> Hits;

	if(UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), TraceStart, TraceEnd, SphereRadius, CollisionProfile, false, ActorsToIgnore, DrawDebugType, Hits, true))
	{
		for (FHitResult Hit : Hits)
		{
			AActor* HitActor = Cast<AActor>(Hit.Actor);
			if (HitActor)
			{
				HitActor->TakeDamage(AttackType.Damage, FDamageEvent{}, GetController(), this);
			}
		}
	}

	if (AttackType.Impulse != 0.f)
	{
		GetCharacterMovement()->AddImpulse(TraceDirection.GetSafeNormal() * AttackType.Impulse);
	}
	PlayAnimation(AttackType.Animation, AttackType.bDisableMovement);
	bIsAttacking = true;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ADemonSlayerCharacter::UpdateAnimation()
{
	//UE_LOG(SideScrollerCharacter, Warning, TEXT("MontageEnded | bIsAttacking: %i"), bIsAttacking);
	
	if (bIsAnimMontagePlaying)
	{
		return;
	}
		
	// Are we moving or standing still?
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	
	UPaperFlipbook* DesiredAnimation =  (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (GetCharacterMovement()->IsFalling())
	{
		DesiredAnimation = FallAnimation;
	}

	if (bIsDead)
	{
		DesiredAnimation = DeathAnimation;
	}
	
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		//UE_LOG(SideScrollerCharacter, Warning, TEXT("Idle | IsLooping: %i"), GetSprite()->IsLooping());
		if (!GetSprite()->IsLooping())
		{
			GetSprite()->SetLooping(true);
		}
		
		if (bIsDead)
		{
			GetSprite()->SetLooping(false);
		}
		GetSprite()->SetFlipbook(DesiredAnimation);
		GetSprite()->PlayFromStart();
	}
}

void ADemonSlayerCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	PlayAnimation(JumpAnimation, false);
}

void ADemonSlayerCharacter::DestroyActor()
{
	this->Destroy();
}

void ADemonSlayerCharacter::OnAnimMontageEnded()
{
	//GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ADemonSlayerCharacter::OnAnimSequenceEnded);
	bIsAnimMontagePlaying = false;
	
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	GetSprite()->SetLooping(true);
	
	if (bIsAttacking == true)
	{
		bIsAttacking = false;
	}
}

void ADemonSlayerCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	CurrentApex = GetActorLocation().Z;
}

void ADemonSlayerCharacter::OnTakeAnyDamageEvent(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	if (Health == 0.f)
	{
		if(OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
	}
	PlayAnimation(DamagedAnimation, true);
	UE_LOG(SideScrollerCharacter, Warning, TEXT("%s | Health: %f"), *this->GetName(), Health);
}

void ADemonSlayerCharacter::OnDeathEvent()
{
	UE_LOG(SideScrollerCharacter, Warning, TEXT("%s is Dead!"), *this->GetName());
	bIsDead = true;
	SetCanBeDamaged(false);
	SetActorEnableCollision(false);
	GetCharacterMovement()->DisableMovement();
	DisableInput(Cast<APlayerController>(GetController()));
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ADemonSlayerCharacter::DestroyActor, DestroyTime);
}

void ADemonSlayerCharacter::Falling()
{
	Super::Falling();
	AirAttackCount = 0;
	GetCharacterMovement()->bNotifyApex = true;
}

void ADemonSlayerCharacter::Attack_Implementation()
{
	//TODO Doesn't work
}

void ADemonSlayerCharacter::OnLandedEvent(const FHitResult& Hit)
{
	float FallHeight = CurrentApex - GetActorLocation().Z;
	if (FallHeight > HardLandingHeight)
	{
		float FallDamage = DefaultFallDamage;
		if (FallDamageCurve)
		{
			FallDamage = FallDamageCurve->GetFloatValue(FallHeight);
		}
		TakeDamage(FallDamage, FDamageEvent{}, GetController(), this);
		PlayAnimation(LandAnimation, true);
	}
	CurrentApex = 0.f;
}

void ADemonSlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacter();	
}

void ADemonSlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &ADemonSlayerCharacter::OnAnimMontageEnded);
	OnTakeAnyDamage.AddDynamic(this, &ADemonSlayerCharacter::OnTakeAnyDamageEvent);
	OnDeath.AddDynamic(this, &ADemonSlayerCharacter::OnDeathEvent);
	LandedDelegate.AddDynamic(this, &ADemonSlayerCharacter::OnLandedEvent);
	
	bIsDead = false;
	Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADemonSlayerCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}
