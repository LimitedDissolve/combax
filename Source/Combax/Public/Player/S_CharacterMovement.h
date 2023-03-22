// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Launch/Resources/Version.h"
#include "S_CharacterMovement.generated.h"

UCLASS()
class COMBAX_API US_CharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	class AS_Character *S_Character;

	//? If we are stepping left, else, right
	bool StepSide;

	//? The multiplier for acceleration when on ground.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float GroundAccelerationMultiplier;

	//? The multiplier for acceleration when in air.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float AirAccelerationMultiplier;

	//? The vector differential magnitude cap when in air.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling")
	float AirSpeedCap;

	//? the minimum step height from moving fast
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite)
	float MinStepHeight;

	//? If the player has already landed for a frame, and breaking may be applied.
	bool bBrakingFrameTolerated;

	//? The target ground speed when running.
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float RunSpeed;

	//? The target ground speed when sprinting.
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SprintSpeed;

	//? The target ground speed when walking slowly.
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float WalkSpeed;

	//? The minimum speed to scale up from for slope movement
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SpeedMultMin;

	//? The maximum speed to scale up to for slope movement
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SpeedMultMax;

	//? The maximum angle we can roll for camera adjust
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	float RollAngle = 0.0f;

	//? Speed of rolling the camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	float RollSpeed = 0.0f;

	//? Speed of rolling the camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	float BounceMultiplier = 0.0f;

	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float AxisSpeedLimit = 6667.5f;

	//? Threshold relating to speed ratio and friction which causes us to catch air
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SlideLimit = 0.5f;

	//? Fraction of uncrouch half-height to check for before doing starting an uncrouch.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	float GroundUncrouchCheckFactor = 0.75f;

public:
	US_CharacterMovement();

	virtual void InitializeComponent() override;
	void OnRegister() override;

	//? Overrides for Source-like movement
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
	virtual void ApplyVelocityBraking(float DeltaTime, float Friction, float BrakingDeceleration) override;
	void PhysFalling(float deltaTime, int32 Iterations);
	bool ShouldLimitAirControl(float DeltaTime, const FVector &FallAcceleration) const override;
	FVector NewFallVelocity(const FVector &InitialVelocity, const FVector &Gravity, float DeltaTime) const override;

	void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	void UpdateSurfaceFriction(bool bIsSliding = false);

	//? Jump overrides
	bool CanAttemptJump() const override;
	bool DoJump(bool bClientSimulation) override;

	void TwoWallAdjust(FVector &OutDelta, const FHitResult &Hit, const FVector &OldHitNormal) const override;
	float SlideAlongSurface(const FVector &Delta, float Time, const FVector &Normal, FHitResult &Hit, bool bHandleImpact = false) override;
	FVector ComputeSlideVector(const FVector &Delta, const float Time, const FVector &Normal, const FHitResult &Hit) const override;
	FVector HandleSlopeBoosting(const FVector &SlideResult, const FVector &Delta, const float Time, const FVector &Normal, const FHitResult &Hit) const override;
	bool ShouldCatchAir(const FFindFloorResult &OldFloor, const FFindFloorResult &NewFloor) override;
	bool IsWithinEdgeTolerance(const FVector &CapsuleLocation, const FVector &TestImpactPoint, const float CapsuleRadius) const override;
	bool IsValidLandingSpot(const FVector &CapsuleLocation, const FHitResult &Hit) const override;
	bool ShouldCheckForValidLandingSpot(float DeltaTime, const FVector &Delta, const FHitResult &Hit) const override;

	void TraceCharacterFloor(FHitResult &OutHit);

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode);

	//~ Do camera roll effect based on velocity
	float GetCameraRoll();

	bool IsBrakingFrameTolerated() const
	{
		return bBrakingFrameTolerated;
	}

	//? Acceleration
	FORCEINLINE FVector GetAcceleration() const
	{
		return Acceleration;
	}

	virtual float GetMaxSpeed() const override;

private:
	float DefaultStepHeight;
	float DefaultWalkableFloorZ;
	float SurfaceFriction;

	bool bHasDeferredMovementMode;
	EMovementMode DeferredMovementMode;
};
