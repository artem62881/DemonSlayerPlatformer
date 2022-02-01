// Fill out your copyright notice in the Description page of Project Settings.


#include "DSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ADSPlayerCharacter::ADSPlayerCharacter()
{
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
}

void ADSPlayerCharacter::OnLandedEvent(const FHitResult& Hit)
{
	if (IsAttacking() && Hit.Actor->IsA<APawn>())
	{
		UE_LOG(LogTemp, Error, TEXT("ADSPlayerCharacter::OnLandedEvent"));
		return;
	}
	Super::OnLandedEvent(Hit);
}

void ADSPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADSPlayerCharacter::ChangeCrouchState()
{
	GetCharacterMovement()->IsCrouching() ? Crouch(false) : UnCrouch(false);
}

void ADSPlayerCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

void ADSPlayerCharacter::UnCrouch(bool bClientSimulation)
{
	Super::UnCrouch(bClientSimulation);
}

void ADSPlayerCharacter::ChangeDownAttackMod()
{
	bDownAttackMod = bDownAttackMod ? false : true;
}

void ADSPlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}