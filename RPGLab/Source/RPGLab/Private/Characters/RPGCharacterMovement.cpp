// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGCharacterMovement.h"

void URPGCharacterMovement::BeginPlay()
{
	ClimbQueryParams.AddIgnoredActor(GetOwner());
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
	}
	else
	{
		CurrentWallHits.Reset();
	}
}
