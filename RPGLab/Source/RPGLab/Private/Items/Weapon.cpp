#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Start = BoxTraceStart->GetComponentLocation();
	FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActor)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	FHitResult HitResult;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start, End, FVector(5.f, 5.f, 5.f),
		BoxComponent->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	UKismetSystemLibrary::DrawDebugSphere(this, HitResult.ImpactPoint, 20.f, 12, FColor::Red, 5.f, 1.f);

	if (HitResult.GetActor())
	{
		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());

		if (HitInterface)
		{
			HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint);
		}

		CreateFields(HitResult.ImpactPoint);
	}

	IgnoreActor.AddUnique(HitResult.GetActor());


}


void AWeapon::Equip(USceneComponent* Inparent, const FName& InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	AttachMeshToSocket(Inparent, InSocketName);
	
}

void AWeapon::AttachMeshToSocket(USceneComponent* Inparent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Inparent, TransformRules, InSocketName);
}
