// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ClimberAnimInstance.h"
#include "Characters/Climber.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UClimberAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Climber = Cast<AClimber>(TryGetPawnOwner());
	if (Climber)
	{
		MovementComponent = Climber->GetCharacterMovement();
	}
}

void UClimberAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		IsFalling_A = MovementComponent->IsFalling();
		if (IsFalling_A && Climber->bPressedJump)
		{
			IsJumping_A = true;
		}
	}

	if (Climber)
	{
		if (Climber->bIsHanging) IsHanging_A = true;
		else IsHanging_A = false;

		if (Climber->bIsClimbingLedge) IsClimbing_A = true;
		else IsClimbing_A = false;

		CanMoveLeft_A = Climber->bCanMoveLeft;
		CanMoveRight_A = Climber->bCanMoveRight;
		IsMovingLeft_A = Climber->bIsMovingLeft;
		IsMovingRight_A = Climber->bIsMovingRight;
		IsLedgeJumping_A = Climber->bIsLedgeJumping;
		IsTurnBack_A = Climber->bIsTurnBack;
	}

	
}
