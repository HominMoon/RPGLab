#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGStates.h"
#include "RPGAnimInstance.generated.h"

UCLASS()
class RPGLAB_API URPGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ARPGCharacter* RPGCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* RPGCharacterMovementComponent;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	double GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | State")
	EWeaponState WeaponState;

};
