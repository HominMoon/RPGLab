// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& Hitlocation)
{
	DirectionalHitReact(Hitlocation);

}

void AEnemy::DirectionalHitReact(const FVector& Hitlocation)
{
	FName Direction;

	//방향성 구하기

	const FVector Forward = GetActorForwardVector();
	const FVector HitLocation(Hitlocation.X, Hitlocation.Y, GetActorLocation().Z);
	const FVector ToHit = (HitLocation - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit); //음수양수 판단위해 사용

	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	Direction = FName("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Direction = FName("FromFront");
	}
	else if (Theta >= 45.f && Theta <= 135.f)
	{
		Direction = FName("FromRight");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Direction = FName("FromLeft");
	}

	PlayHitReactMontage(Direction);
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitMontage)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
}

