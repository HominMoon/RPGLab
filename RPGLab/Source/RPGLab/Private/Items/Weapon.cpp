#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(BoxComponent);

	BoxTraceEnd = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(BoxComponent);
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

	FHitResult HitResult;

	UKismetSystemLibrary::BoxTraceSingle(this,
		Start, End, FVector(5.f, 5.f, 5.f),
		BoxComponent->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	UE_LOG(LogTemp, Warning, TEXT("!!!"));

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
