
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ClimberStates.h"
#include "Climber.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class APlayerController;

UCLASS()
class RPGLAB_API AClimber : public ACharacter
{
	GENERATED_BODY()

public:
	AClimber();
	virtual void Tick(float DeltaTime) override;
	void HipToLedge();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerMoveState MoveState = EPlayerMoveState::EPM_Idle;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UCharacterMovementComponent* MovementComponent;

	APlayerController* CurrentPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ForwardTracer();
	void HeightTracer();
	void LeftTracer();
	void RightTracer();
	void JumpLeftTracer();
	void JumpRightTracer();

	UFUNCTION(BlueprintCallable)
	void GrabLedge();
	UFUNCTION(BlueprintCallable)
	void MoveInLedge();
	void ExitLedge();

	UFUNCTION(BlueprintCallable)
	void ClimbLedge();
	UFUNCTION(BlueprintCallable)
	FVector GrabLedgeMoveToLocation();
	UFUNCTION(BlueprintCallable)
	FRotator GrabLedgeMoveToRotation();
	UFUNCTION(BlueprintCallable)
	void GrabLedgeMoveToLocationFinished();
	UFUNCTION(BlueprintCallable)
	void JumpSideLedge();

	void PlayClimbMontage();
	void PlayJumpSideLedgeMontage();

private:

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* LeftArrow;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* RightArrow;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* LeftLedgeArrow;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* RightLedgeArrow;


	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* ClimbMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* JumpSideLedgeMontage;

	FVector WallLocation;
	FVector WallNormal;
	FVector HeightLocation;

	ETraceTypeQuery LedgeTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

public:	
	UPROPERTY(BlueprintReadOnly)
	bool bIsHanging = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbingLedge = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanGrab = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanMoveLeft = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanMoveRight = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanJumpLeft = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanJumpRight = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsMovingLeft = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsMovingRight = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsLedgeJumping = false;

	bool WallChecked = false;
	bool HeightChecked = false;
};
