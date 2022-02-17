// Fill out your copyright notice in the Description page of Project Settings.


#include "DSPlayerController.h"

#include "DemonSlayer/Characters/DSPlayerCharacter.h"

void ADSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	CachedPlayer = Cast<ADSPlayerCharacter>(InPawn);
	CreateAndInitWidgets();
}

void ADSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//auto Player = Cast<ADSPlayerCharacter>(GetOwner());
	//checkf(Player->IsA<ADSPlayerCharacter>(), TEXT("void ADSPlayerController::BeginPlay() PlayerController can be used only with ADSPlayerCharacter"));
	//SetPawn(Player);
}

void ADSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Jump", IE_Pressed, this, &ADSPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ADSPlayerController::StopJumping);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &ADSPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Crouch", IE_Released, this, &ADSPlayerController::ChangeCrouchState);
	InputComponent->BindAction("Attack", IE_Pressed, this, &ADSPlayerController::Attack);
	InputComponent->BindAction("DownAttackMod", IE_Pressed, this, &ADSPlayerController::ChangeDownAttackMod);
	InputComponent->BindAction("DownAttackMod", IE_Released, this, &ADSPlayerController::ChangeDownAttackMod);
	//PlayerInputComponent->BindAction("UltimateAttackModifier", IE_Pressed, this, &ADSPlayerCharacter::EnableUltimateAttack);
	InputComponent->BindAxis("MoveRight", this, &ADSPlayerController::MoveRight);

	InputComponent->BindTouch(IE_Pressed, this, &ADSPlayerController::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ADSPlayerController::TouchStopped);
}

void ADSPlayerController::Jump()
{
	if (CachedPlayer)
	{
		CachedPlayer->Jump();
	}
}

void ADSPlayerController::StopJumping()
{
	if (CachedPlayer)
	{
		CachedPlayer->StopJumping();
	}
}

void ADSPlayerController::ChangeCrouchState()
{
	if (CachedPlayer)
	{
		CachedPlayer->ChangeCrouchState();
	}
}

void ADSPlayerController::MoveRight(float Value)
{
	if (CachedPlayer)
	{
		CachedPlayer->MoveRight(Value);
	}
}

void ADSPlayerController::Attack()
{
	if (CachedPlayer)
	{
		CachedPlayer->Attack();
	}
}

void ADSPlayerController::ChangeDownAttackMod()
{
	if (CachedPlayer)
	{
		CachedPlayer->ChangeDownAttackMod();
	}
}

void ADSPlayerController::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (CachedPlayer)
	{
		CachedPlayer->Jump();
	}
}

void ADSPlayerController::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (CachedPlayer)
	{
		CachedPlayer->StopJumping();
	}
}

void ADSPlayerController::CreateAndInitWidgets()
{
	if (!PlayerHUDWidget)
	{
		PlayerHUDWidget = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

}
