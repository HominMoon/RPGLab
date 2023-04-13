// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ClimberAnimInstance.generated.h"


class AClimber;
class UCharacterMovementComponent;

UCLASS()
class RPGLAB_API UClimberAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	AClimber* Climber;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	double GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsJumping_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsHanging_A;
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool IsClimbing_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool CanMoveLeft_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool CanMoveRight_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsMovingLeft_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsMovingRight_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsLedgeJumping_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsLedgeJumpLeft_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsLedgeJumpRight_A;

	
};
