// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ObjectMacros.h"
#include "RPGCharacterMovement.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_Climbing UMETA(DisplayName = "Climbing"),
	CMOVE_MAX UMETA(Hidden),
};


UCLASS()
class RPGLAB_API URPGCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TryClimbing();
	void CancelClimbing();

	UFUNCTION(BlueprintPure)
	bool IsClimbing() const;
	UFUNCTION(BlueprintPure)
	FVector GetClimbSurfaceNormal() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class AWeapon* IgnoredWeapon;
	UPROPERTY(VisibleAnywhere)
	class ARPGCharacter* RPGCharacter;

private:
	void SweepAndStoreWallHits();
	bool CanStartClimbing();
	bool EyeHeightTrace(const float TraceDistance) const;
	bool IsFacingSurface(const float SurfaceVerticalDot) const;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbing(float deltaTime, int32 Iterations);
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	void ComputeSurfaceInfo();
	void ComputeClimbingVelocity(float deltaTime);
	bool ShouldStopClimbing();
	void StopClimbing(float deltaTime, int32 Iterations);
	void MoveAlongClimbingSurface(float deltaTime);
	void SnapToClimbingSurface(float deltaTime) const;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
	FQuat GetClimbingRotation(float deltaTime) const;

	bool ClimbDownToFloor() const;
	bool CheckFloor(FHitResult& FloorHit) const;
	bool TryClimbUpLedge() const;
	bool HasReachedEdge() const;
	bool IsLocationWalkable(const FVector& CheckLocation) const;
	bool CanMoveToLedgeClimbLocation() const;


	UPROPERTY(Category = "Character Movement: Climbing", EditDefaultsOnly)
	UAnimMontage* LedgeClimbMontage;
	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing")
	int CollisionCapsuleRadius = 50;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing")
	int CollisionCapsuleHalfHeight = 72;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing", meta = (ClampMin = "1.0", ClampMax = "75.0"))
	float MinorHorizontalDegreesToStartClimbing = 25.f;
	
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "10.0", ClampMax = "500.0"))
	float MaxClimbingSpeed = 120.f;
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "10.0", ClampMax = "2000.0"))
	float MaxClimbingAcceleration = 380.f;
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "3000.0"))
	float BrakingDecelerationClimbing = 550.f;
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "12.0"))
	int ClimbingRotationSpeed = 6;
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "60.0"))
	float ClimbingSnapSpeed = 4.f;
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "80.0"))
	float DistanceFromSurface = 45.f;
	UPROPERTY(Category = "Character Movement: Climbing", EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "500.0"))
	float FloorCheckDistance = 100.f;

	bool bWantsToClimb = false;
	float ClimbingCollisionShrinkAmount = 30.f;
	FVector CurrentClimbingNormal;
	FVector CurrentClimbingPosition;

	TArray<FHitResult> CurrentWallHits;

	FCollisionQueryParams ClimbQueryParams;

	
};
