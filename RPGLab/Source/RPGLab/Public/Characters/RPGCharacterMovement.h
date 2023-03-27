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
	class AWeapon* Weapon;

private:
	void SweepAndStoreWallHits();
	bool CanStartClimbing();
	bool EyeHeightTrace(const float TraceDistance) const;
	bool IsFacingSurface(const float SurfaceVerticalDot) const;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	void PhysClimbing(float deltaTime, int32 Iterations);
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing")
	int CollisionCapsuleRadius = 50;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing")
	int CollisionCapsuleHalfHeight = 72;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing", meta = (ClampMin = "1.0", ClampMax = "75.0"))
	float MinorHorizontalDegreesToStartClimbing = 25.f;

	bool bWantsToClimb = false;
	float ClimbingCollisionShrinkAmount = 30.f;

	TArray<FHitResult> CurrentWallHits;

	FCollisionQueryParams ClimbQueryParams;

	
};
