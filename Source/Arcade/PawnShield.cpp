// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PawnShield.h"
#include "PlayerPawn.h"
#include "ConstructorHelpers.h"

APawnShield::APawnShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** mesh  */
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetGenerateOverlapEvents(false);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(Sphere.Object);
	}
}

void APawnShield::Activate(class APlayerPawn* PlayerPawn)
{
	if (PlayerPawn)
	{
		/** shield owner can not be damaged until shield exists  */
		PlayerPawn->bCanBeDamaged = false;
		Player = PlayerPawn;

		AttachToActor(PlayerPawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		/** destroy shield by timer  */
		FTimerHandle DeactivateTimer;
		GetWorldTimerManager().SetTimer(DeactivateTimer, this, &APawnShield::Deactivate, DeactivationDelay, false);
	}
}

void APawnShield::Deactivate()
{
	if ( Player && ! Player->IsPendingKill() && ! IsPendingKill() )
	{
		/** shield's owner can receive damage  */
		Player->bCanBeDamaged = true;

		Destroy();
	}
}

