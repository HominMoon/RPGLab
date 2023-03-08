// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPGAnimInstance.h"
#include "Characters/RPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URPGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RPGCharacter = Cast<ARPGCharacter>(TryGetPawnOwner());
	if (RPGCharacter)
	{
		RPGCharacterMovementComponent = RPGCharacter->GetCharacterMovement();
	}
}

void URPGAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (RPGCharacterMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(RPGCharacterMovementComponent->Velocity);
		IsFalling = RPGCharacterMovementComponent->IsFalling();
		WeaponState = RPGCharacter->GetWeaponState();
	}
}
