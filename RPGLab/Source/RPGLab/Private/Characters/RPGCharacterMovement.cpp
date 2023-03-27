// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacterMovement.h"
#include "Characters/RPGCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapon.h"

void URPGCharacterMovement::BeginPlay()
{
	Super::BeginPlay();

	ClimbQueryParams.AddIgnoredActor(GetOwner());
	ClimbQueryParams.AddIgnoredActor(Weapon);
}

void URPGCharacterMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SweepAndStoreWallHits();
}

void URPGCharacterMovement::SweepAndStoreWallHits()
{
	const FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CollisionCapsuleRadius, CollisionCapsuleHalfHeight);

	const FVector StartOffset = UpdatedComponent->GetForwardVector() * 20;

	const FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	const FVector End = Start + UpdatedComponent->GetForwardVector();

	TArray<FHitResult> HitResults;

	const bool HitWall = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldStatic,
		CollisionShape,
		ClimbQueryParams
	);

	if (HitWall)
	{
		CurrentWallHits = HitResults;

		for (FHitResult Hitresult : HitResults)
		{
			DrawDebugPoint(GetWorld(),
				Hitresult.ImpactPoint,
				5.f,
				FColor::Red
			);
		}
	}
	else
	{
		CurrentWallHits.Reset();
	}
}

bool URPGCharacterMovement::CanStartClimbing()
{
	for (FHitResult& HitResult : CurrentWallHits)
	{
		const FVector HorizontalNormal = HitResult.Normal.GetSafeNormal2D();
		const float HorizontalDot = FVector::DotProduct(UpdatedComponent->GetForwardVector(), -HorizontalNormal);
		const float VerticalDot = FVector::DotProduct(HitResult.Normal, HorizontalNormal);

		const float HorizontalDegrees = FMath::RadiansToDegrees(FMath::Acos(HorizontalDot));
		const bool bIsCeiling = FMath::IsNearlyZero(VerticalDot);

		if (HorizontalDegrees <= MinorHorizontalDegreesToStartClimbing && !bIsCeiling && EyeHeightTrace(VerticalDot))
		{
			return true;
		}
	}
	return false;
}

bool URPGCharacterMovement::EyeHeightTrace(const float TraceDistance) const
{
	FHitResult UpperEdgeHit;

	const FVector Start = UpdatedComponent->GetComponentLocation() +
		(UpdatedComponent->GetUpVector() * GetCharacterOwner()->BaseEyeHeight);
	const FVector End = Start + (UpdatedComponent->GetForwardVector() * TraceDistance);
	
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::White
		);

	return GetWorld()->LineTraceSingleByChannel(
		UpperEdgeHit,
		Start,
		End,
		ECollisionChannel::ECC_WorldStatic,
		ClimbQueryParams
	);
}

bool URPGCharacterMovement::IsFacingSurface(const float SurfaceVerticalDot) const
{
	constexpr float BaseLength = 80;
	const float SteepnessMultiplier = 1 + (1 - SurfaceVerticalDot) * 5;

	return EyeHeightTrace(BaseLength * SteepnessMultiplier);
}

void URPGCharacterMovement::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (bWantsToClimb)
	{
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_Climbing);
	}
}

void URPGCharacterMovement::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (CustomMovementMode == ECustomMovementMode::CMOVE_Climbing)
	{
		PhysClimbing(deltaTime, Iterations);
	}
}

void URPGCharacterMovement::PhysClimbing(float deltaTime, int32 Iterations)
{
}

void URPGCharacterMovement::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (IsClimbing())
	{
		bOrientRotationToMovement = false;

		UCapsuleComponent* Capsule = CharacterOwner->GetCapsuleComponent();
		Capsule->SetCapsuleHalfHeight(Capsule->GetUnscaledCapsuleHalfHeight() - ClimbingCollisionShrinkAmount);
	}

	const bool bWasClimbing = PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Climbing;

	if (bWasClimbing)
	{
		bOrientRotationToMovement = true;

		const FRotator StandRotation = FRotator(0, UpdatedComponent->GetComponentRotation().Yaw, 0);
		UpdatedComponent->SetRelativeRotation(StandRotation);

		UCapsuleComponent* Capsule = CharacterOwner->GetCapsuleComponent();
		Capsule->SetCapsuleHalfHeight(Capsule->GetUnscaledCapsuleHalfHeight() + ClimbingCollisionShrinkAmount);

		StopMovementImmediately();
	}
}

void URPGCharacterMovement::TryClimbing()
{
	if (CanStartClimbing())
	{
		bWantsToClimb = true;
	}
}
void URPGCharacterMovement::CancelClimbing()
{
	bWantsToClimb = false;
}

bool URPGCharacterMovement::IsClimbing() const
{
	return MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == ECustomMovementMode::CMOVE_Climbing;
}

FVector URPGCharacterMovement::GetClimbSurfaceNormal() const
{
	if (CurrentWallHits.Num() > 0)
	{
		return CurrentWallHits[0].Normal;
	}
	else
	{
		return FVector::Zero();
	}
}
