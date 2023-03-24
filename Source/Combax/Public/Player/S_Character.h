// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "S_Character.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class US_CharacterMovement;

inline float SimpleSpline(float Value)
{
	const float ValueSquared = Value * Value;
	return (3.0f * ValueSquared - 2.0f * ValueSquared * Value);
}

UCLASS(config = game)
class COMBAX_API AS_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent *Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext *DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *MoveAction;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	void Tick(float DeltaTime) override;

	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	// void Jump() override;
	// virtual void ClearJumpInput(float DeltaTime) override;
	// virtual void StopJumping() override;

	virtual bool CanJumpInternal_Implementation() const override;
	void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode) override;

	void RecalculateBaseEyeHeight() override;

	float GetLastJumpTime()
	{
		return LastJumpTime;
	}

private:
	US_CharacterMovement *MovementPtr;

	float DefaultBaseEyeHeight;
	float LastJumpTime;
	float LastJumpBoostTime;
	float MaxJumpTime;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Camera")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Gameplay")
	bool bAutoBunnyhop;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Damage")
	float MinLandBounceSpeed;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Damage")
	float MinSpeedForFallDamage;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Damage")
	float CapDamageMomentumZ = 0.f;

	bool bIsSprinting;
	bool bWantsToWalk;
	bool bDeferJumpStop;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction *LookAction;

public:
	USkeletalMeshComponent *GetMesh1P() const { return Mesh1P; }
	UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	virtual void BeginPlay();

public:
	AS_Character(const FObjectInitializer &ObjectInitializer);

#pragma region Mutators
	UFUNCTION()
	bool IsSprinting() const
	{
		return bIsSprinting;
	}
	UFUNCTION()
	bool DoesWantToWalk() const
	{
		return bWantsToWalk;
	}
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE float GetBaseTurnRate() const
	{
		return BaseTurnRate;
	};
	UFUNCTION(Category = "PB Setters", BlueprintCallable)
	void SetBaseTurnRate(float val)
	{
		BaseTurnRate = val;
	};
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE float GetBaseLookUpRate() const
	{
		return BaseLookUpRate;
	};
	UFUNCTION(Category = "PB Setters", BlueprintCallable)
	void SetBaseLookUpRate(float val)
	{
		BaseLookUpRate = val;
	};
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE bool GetAutoBunnyhop() const
	{
		return bAutoBunnyhop;
	};
	UFUNCTION(Category = "PB Setters", BlueprintCallable)
	void SetAutoBunnyhop(bool val)
	{
		bAutoBunnyhop = val;
	};
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE US_CharacterMovement *GetMovementPtr() const
	{
		return MovementPtr;
	};

#pragma endregion Mutators

	float GetDefaultBaseEyeHeight() const
	{
		return DefaultBaseEyeHeight;
	}
	float GetMinLandBounceSpeed() const { return MinLandBounceSpeed; }

	UFUNCTION(Category = "Player Movement", BlueprintPure)
	float GetMinSpeedForFallDamage() const { return MinSpeedForFallDamage; };
};
