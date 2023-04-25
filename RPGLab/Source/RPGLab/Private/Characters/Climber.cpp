// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Climber.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
AClimber::AClimber()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm);

	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
	LeftArrow->SetupAttachment(GetRootComponent());
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));
	RightArrow->SetupAttachment(GetRootComponent());
	LeftLedgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Ledge Arrow"));
	LeftLedgeArrow->SetupAttachment(GetRootComponent());
	RightLedgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Ledge Arrow"));
	RightLedgeArrow->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AClimber::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetCharacterMovement();
	CurrentPlayerController = Cast<APlayerController>(GetController());

	if (CurrentPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CurrentPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}

	}
}

void AClimber::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (bIsHanging)
	{
		if (MoveVector.X > 0)
		{
			bIsMovingRight = true;
			bIsMovingLeft = false;
		}
		else if (MoveVector.X < 0)
		{
			bIsMovingLeft = true;
			bIsMovingRight = false;
		}
		else
		{
			bIsMovingLeft = false;
			bIsMovingRight = false;
		}

		return;
	}


	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void AClimber::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

void AClimber::ForwardTracer()
{
	double ValueX = GetActorForwardVector().X * 150;
	double ValueY = GetActorForwardVector().Y * 150;
	FVector ForwardTraceValue(FVector(ValueX, ValueY, GetActorForwardVector().Z));
	const FVector Start = GetActorLocation();
	const FVector End = Start + ForwardTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		20.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		WallLocation = HitResult.Location;
		WallNormal = HitResult.ImpactNormal;
		ActorsToIgnore.AddUnique(this);
		WallChecked = true;
	}
	else
	{
		WallChecked = false;
	}

	return;
}

void AClimber::HeightTracer()
{
	FVector HeightTraceValue(FVector(0.f, 0.f, 100.f));
	FVector ForwardTraceValue = GetActorForwardVector() * 70.f;
	FVector End = GetActorLocation() + ForwardTraceValue;
	FVector Start = End + HeightTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		20.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		FResult = HitResult;
		HeightLocation = HitResult.ImpactPoint;
		HeightChecked = true;
	}
	else
	{
		HeightChecked = false;
	}
}

void AClimber::LeftTracer()
{
	FVector TraceValue(LeftArrow->GetComponentLocation());

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		TraceValue,
		TraceValue,
		40.f,
		80.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit) bCanMoveLeft = true;
	else bCanMoveLeft = false;


}

void AClimber::RightTracer()
{
	FVector TraceValue(RightArrow->GetComponentLocation());

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		TraceValue,
		TraceValue,
		40.f,
		80.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit) bCanMoveRight = true;
	else bCanMoveRight = false;
}

void AClimber::JumpLeftTracer()
{
	FVector TraceValue(LeftLedgeArrow->GetComponentLocation());

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		TraceValue,
		TraceValue,
		20.f,
		60.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		if (!bCanMoveLeft) bCanJumpLeft = true;
		else bCanJumpLeft = false;
	}
	else bCanJumpLeft = false;
}

void AClimber::JumpRightTracer()
{
	FVector TraceValue(RightLedgeArrow->GetComponentLocation());

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		TraceValue,
		TraceValue,
		20.f,
		60.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		if (!bCanMoveRight) bCanJumpRight = true;
		else bCanJumpRight = false;
	}
	else bCanJumpRight = false;
}

void AClimber::GrabLedge()
{
	if (!MovementComponent || bIsHanging) return;

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	bIsHanging = true;
	TEnumAsByte<EMoveComponentAction::Type> Action = EMoveComponentAction::Move;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "GrabLedgeMoveToLocationFinished";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo(
		RootComponent,
		GrabLedgeMoveToLocation(),
		GrabLedgeMoveToRotation(),
		false,
		false,
		0.15f,
		false,
		Action,
		LatentInfo
	);

}

void AClimber::MoveInLedge()
{		
	const FVector Current = GetActorLocation();
	FVector Target;
	
	if (bCanMoveLeft && bIsMovingLeft) 
	{ 
		Target = Current - GetActorRightVector() * 20.f; 
		SetActorLocation(FMath::VInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), 5.f));
	}
	if (bCanMoveRight && bIsMovingRight) 
	{ 
		Target = Current + GetActorRightVector() * 20.f;
		SetActorLocation(FMath::VInterpTo(Current, Target, GetWorld()->GetDeltaSeconds(), 5.f));
	}
}

void AClimber::ExitLedge()
{
	bIsHanging = false;
	FResult.Reset();
	FRotator NewRotation(FRotator(0.f, GetActorRotation().Yaw, GetActorRotation().Roll));
	SetActorRotation(NewRotation);
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
}

void AClimber::ClimbLedge()
{
	if (bIsClimbingLedge) return;
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	bIsClimbingLedge = true;
	bIsHanging = false;

	PlayClimbMontage();
}

FVector AClimber::GrabLedgeMoveToLocation()
{
	double LocationX = WallLocation.X + WallNormal.X * 22.f;
	double LocationY = WallLocation.Y + WallNormal.Y * 22.f;
	double LocationZ = HeightLocation.Z - 120.f;

	FVector LocationF(FVector(LocationX, LocationY, LocationZ));

	UE_LOG(LogTemp, Warning, TEXT("GLMTL"));

	return LocationF;
}

FRotator AClimber::GrabLedgeMoveToRotation()
{	
	FRotator Rotation(FRotator(-WallNormal.Rotation().Pitch, WallNormal.Rotation().Yaw - 180.f, WallNormal.Rotation().Roll));
	return Rotation;
}

void AClimber::GrabLedgeMoveToLocationFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("!!"));
	MovementComponent->StopMovementImmediately();

	AttachToActor(FResult.GetActor(), AttachmentRules);
}

void AClimber::JumpSideLedge()
{
	UE_LOG(LogTemp, Warning, TEXT("JS"));

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	bIsHanging = true;
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "GrabLedge";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	PlayJumpSideLedgeMontage();

	UKismetSystemLibrary::Delay(this, 2.f, LatentInfo);
}

void AClimber::Heal()
{
	if (Health >= 10.f) return;

	Health += 1.f;
}

void AClimber::PlayClimbMontage()
{
	UAnimInstance* Instance = GetMesh()->GetAnimInstance();
	if (Instance && ClimbMontage)
	{
		Instance->Montage_Play(ClimbMontage);
	}
}

void AClimber::PlayJumpSideLedgeMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("PJ"));
	FName SectionName;
	if (bIsMovingLeft) SectionName = "Left";
	else if (bIsMovingRight) SectionName = "Right";
	else return;

	UAnimInstance* Instance = GetMesh()->GetAnimInstance();
	if (Instance)
	{
		Instance->Montage_Play(JumpSideLedgeMontage);
		Instance->Montage_JumpToSection(SectionName);
	}
}

// Called every frame
void AClimber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForwardTracer();
	HeightTracer();
	//HipToLedge();

	if (CurrentPlayerController)
	{
		if (bIsHanging)
		{

			LeftTracer();
			RightTracer();
			MoveInLedge();

			if (bCanMoveLeft) bCanJumpLeft = false;
			else JumpLeftTracer();

			if (bCanMoveRight) bCanJumpRight = false;
			else JumpRightTracer();

			if (bCanJumpLeft || bCanJumpRight) bIsLedgeJumping = true;
			else bIsLedgeJumping = false;

			//if (CurrentPlayerController->WasInputKeyJustPressed(EKeys::S)) ExitLedge();
		}
		else
		{
			DetachFromActor(DetachmentRules);
			bIsMovingLeft = false;
			bIsMovingRight = false;
		}
	}
}

void AClimber::HipToLedge()
{
	double SpineLocationZ = GetMesh()->GetSocketLocation(FName(TEXT("SpineSocket"))).Z;
	double HeightLocationZ = HeightLocation.Z;

	double Range = SpineLocationZ - HeightLocationZ;

	if (UKismetMathLibrary::InRange_FloatFloat(Range, -70.f, 20.f) && !bIsClimbingLedge && WallChecked && HeightChecked)
	{
		bCanGrab = true;
		GrabLedge();
	}
	else bCanGrab = false;
}

// Called to bind functionality to input
void AClimber::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AClimber::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AClimber::Look);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	}

}

