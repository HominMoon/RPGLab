#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class RPGLAB_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(EditAnywhere)
	USceneComponent* BoxTraceEnd;

	

	UFUNCTION()
	void OnBoxOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

public:
	TArray<AActor*> IgnoreActor;

	void Equip(USceneComponent* Inparent, const FName& InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName);

};
