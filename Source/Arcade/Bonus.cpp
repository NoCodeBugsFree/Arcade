// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Bonus.h"
#include "Components/SphereComponent.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"

ABonus::ABonus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** mesh  */
	BonusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BonusMesh"));
	SetRootComponent(BonusMesh);
	BonusMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		BonusMesh->SetStaticMesh(Sphere.Object);
	}

	/* sphere collision  */
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(50.f);
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	/** set the explosion emitter  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Assest/Particles/P_OnBonusCollected"));
	if (ParticleSystem.Object)
	{
		OnBonusCollectedEmitterTemplate = ParticleSystem.Object;
	}
}

void ABonus::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor))
	{
		BonusCollected(PlayerPawn);
	}
}

void ABonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float WorldMoveOffset = -100.f;
	WorldMoveOffset *= DeltaTime;
	AddActorWorldOffset(FVector(WorldMoveOffset, 0.f, 0.f));
}

void ABonus::BonusCollected_Implementation(APlayerPawn* PlayerPawn)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnBonusCollectedEmitterTemplate, GetActorTransform());

	Destroy();
}

