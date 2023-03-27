#pragma once

#include "Characters/RPGCharacter.h"

#include "Characters/RPGCharacterMovement.h"

#include "Items/Weapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Items/Item.h"

ARPGCharacter::ARPGCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URPGCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = Cast<URPGCharacterMovement>(GetCharacterMovement());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm);

}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	EquippedWeapon->AttachMeshToSocket(GetMesh(), "SpineWeaponSocket");

}

void ARPGCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState == EActionState::EAS_Attacking || ActionState == EActionState::EAS_Equipping) { return; }
	FVector2D MoveVector = Value.Get<FVector2D>();

	FVector ForwardDirection;
	FVector RightDirection;

	if (MovementComponent->IsClimbing())
	{
		ForwardDirection = FVector::CrossProduct(MovementComponent->GetClimbSurfaceNormal(), -GetActorRightVector());
		RightDirection = FVector::CrossProduct(MovementComponent->GetClimbSurfaceNormal(), GetActorUpVector());
	}
	else
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void ARPGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

void ARPGCharacter::EKeyPressed()
{
	if (EquippedWeapon)
	{
		if (CanArm())
		{
			PlayEquipMontage("Equip");
			WeaponState = EWeaponState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_Equipping;
		}
		else if (CanDisArm())
		{
			PlayEquipMontage("UnEquip");
			WeaponState = EWeaponState::ECS_UnEquipped;
			ActionState = EActionState::EAS_Equipping;
		}
	}
}

void ARPGCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ARPGCharacter::Climb()
{
	MovementComponent->TryClimbing();
}

void ARPGCharacter::CancelClimb()
{
	MovementComponent->CancelClimbing();
}

bool ARPGCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Idle && WeaponState == EWeaponState::ECS_UnEquipped;
}

bool ARPGCharacter::CanDisArm()
{
	return ActionState == EActionState::EAS_Idle && WeaponState == EWeaponState::ECS_EquippedOneHandedWeapon;
}

bool ARPGCharacter::CanAttack()
{
	return WeaponState == EWeaponState::ECS_EquippedOneHandedWeapon && ActionState == EActionState::EAS_Idle;
}

void ARPGCharacter::Arm()
{
	EquippedWeapon->AttachMeshToSocket(GetMesh(), "RightHandSocket");
}

void ARPGCharacter::DisArm()
{
	EquippedWeapon->AttachMeshToSocket(GetMesh(), "SpineWeaponSocket");
}

void ARPGCharacter::EndEquip()
{
	ActionState = EActionState::EAS_Idle;
}

void ARPGCharacter::EndAttack()
{
	ActionState = EActionState::EAS_Idle;
}

void ARPGCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT(" !!"));
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ARPGCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && EquipMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT(" !!"));
		AnimInstance->Montage_Play(AttackMontage);
	}
}

void ARPGCharacter::SetBoxCollisionEnabled(ECollisionEnabled::Type CollsionType)
{
	EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollsionType);
	EquippedWeapon->IgnoreActor.Empty();
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyPressedAction, ETriggerEvent::Triggered, this, &ARPGCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Triggered, this, &ARPGCharacter::Climb);
		EnhancedInputComponent->BindAction(CancelClimbAction, ETriggerEvent::Triggered, this, &ARPGCharacter::CancelClimb);
	}
}

