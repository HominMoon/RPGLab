#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "RPGStates.h"
#include "RPGCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class AWeapon;

UCLASS()
class RPGLAB_API ARPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARPGCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState = EWeaponState::ECS_UnEquipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Idle;

	UFUNCTION(BlueprintPure)
	FORCEINLINE URPGCharacterMovement* GetCustomCharacterMovement() const { return MovementComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	URPGCharacterMovement* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EKeyPressedAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ClimbAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CancelClimbAction;

	UPROPERTY(EditAnywhere)
	bool IsHangingC = false;

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();
	void Climb();
	void CancelClimb();

	bool CanArm();
	bool CanDisArm();
	bool CanAttack();
	UFUNCTION(BlueprintCallable)
	void Arm();
	UFUNCTION(BlueprintCallable)
	void DisArm();
	UFUNCTION(BlueprintCallable)
	void EndEquip();
	UFUNCTION(BlueprintCallable)
	void EndAttack();

	void PlayEquipMontage(FName SectionName);
	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void SetBoxCollisionEnabled(ECollisionEnabled::Type CollsionType);

private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	AItem* OverlappingItem;
	UPROPERTY(EditAnywhere)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:	
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	UFUNCTION()
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	FORCEINLINE AWeapon* GetRPGWeapon() { return EquippedWeapon; }

};
