// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

AProjectile::AProjectile() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* sphere collision  */
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetSphereRadius(10.f);
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	/** Flakky's collision handle method  */
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/** mesh  */
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionProfileName("NoCollision");
	StaticMeshComponent->SetGenerateOverlapEvents(false);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (Sphere.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(Sphere.Object);
	}

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	TrailParticles->SetupAttachment(RootComponent);
	TrailParticles->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TrailParticles->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	/** set the explosion emitter  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemTemplate(TEXT("/Game/Assest/Particles/P_EngineFire"));
	if (ParticleSystemTemplate.Object)
	{
		TrailParticles->SetTemplate(ParticleSystemTemplate.Object);
	}

	InitialLifeSpan = 3.f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	/** Flakky's collision handle method  */
	if (GetOwner())
	{
		UBoxComponent* OwnerCollision = GetOwner()->FindComponentByClass<UBoxComponent>();
		if (OwnerCollision)
		{
			SphereComponent->IgnoreComponentWhenMoving(OwnerCollision, true);
			OwnerCollision->IgnoreComponentWhenMoving(SphereComponent, true);

			SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalOffset(FVector(Speed * DeltaTime, 0.f, 0.f));
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && !OtherActor->IsPendingKill())
	{	
		/** apply damage   */
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

		Destroy();
	}
}
