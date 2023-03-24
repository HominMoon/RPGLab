// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPGCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class RPGLAB_API URPGCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	void SweepAndStoreWallHits();

	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing")
	int CollisionCapsuleRadius = 50;
	UPROPERTY(EditAnywhere, Category = "Character Movement: Climbing")
	int CollisionCapsuleHalfHeight = 72;

	TArray<FHitResult> CurrentWallHits;

	FCollisionQueryParams ClimbQueryParams;

	
};
