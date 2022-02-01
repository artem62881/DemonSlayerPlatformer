// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemonSlayerCharacter.h"
#include "DSPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DEMONSLAYER_API ADSPlayerCharacter : public ADemonSlayerCharacter
{
	GENERATED_BODY()
	
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

protected:
	virtual void Crouch(bool bClientSimulation) override;
	virtual void UnCrouch(bool bClientSimulation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DownAttackUpImpulse = 30000.f;
	
public:
	ADSPlayerCharacter();

	void ChangeCrouchState();

	/** Called for side to side input */
	void MoveRight(float Value);

	void ChangeDownAttackMod();
	
	virtual void OnLandedEvent(const FHitResult& Hit) override;
	
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
	bool GetDownAttackMod() const { return bDownAttackMod; };
	
private:
	bool bDownAttackMod = false;
};
