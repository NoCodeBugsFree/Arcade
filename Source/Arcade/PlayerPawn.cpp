// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PlayerPawn.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ShootComponent.h"
#include "HealthComponent.h"
#include "Materials/MaterialInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

APlayerPawn::APlayerPawn()
{
	/* box collision  */
	BoxPawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxPawnCollision);
	BoxPawnCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxPawnCollision->SetCollisionProfileName("Pawn");
	BoxPawnCollision->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	/** mesh  */
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(RootComponent);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		PawnMesh->SetStaticMesh(Cube.Object);
	}
	
	/* camera component  */
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));
	CameraComponent->PostProcessSettings.AutoExposureMinBrightness = 1.f;
	CameraComponent->PostProcessSettings.AutoExposureMaxBrightness = 1.f;
	
	/**  set blink material */
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialTemplate(TEXT("/Game/Assest/Materials/M_Recover"));
	if (MaterialTemplate.Succeeded())
	{
		RecoverMaterial = MaterialTemplate.Object;
	}

	/* shoot component  */
	ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("ShootComponent"));

	/* health component  */
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	/** set the explosion emitter  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Assest/Particles/P_Explode"));
	if (ParticleSystem.Object)
	{
		ExplodeParticles = ParticleSystem.Object;
	}

	EngineFireParcticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EngineFireParcticles"));
	EngineFireParcticles->SetupAttachment(RootComponent);
	EngineFireParcticles->SetRelativeLocation(FVector(-70.f, 0.f, 0.f));
	EngineFireParcticles->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	/** set the explosion emitter  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystemEngine(TEXT("/Game/Assest/Particles/P_EngineFire"));
	if (ParticleSystemEngine.Object)
	{
		EngineFireParcticles->SetTemplate(ParticleSystemEngine.Object);
	}
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnHealthEnded.AddDynamic(this, &APlayerPawn::ExplodePawn);

	InitialMaterial = PawnMesh->GetMaterial(0);
}

void APlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (APlayerController* TestPC = Cast<APlayerController>(Controller))
	{
		PC = TestPC;
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindTouch(IE_Repeat, this, &APlayerPawn::OnTouchMove);
	InputComponent->BindTouch(IE_Pressed, this, &APlayerPawn::OnTouchPressed);
}

float APlayerPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = 0.f;
	
	/** only take damage if we vulnerable  */
	if (bCanBeDamaged)
	{
		ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		OnPawnDamaged.Broadcast();
	}

	return ActualDamage;
}

void APlayerPawn::OnTouchMove(ETouchIndex::Type Index, FVector Location)
{
	/** find this frame delta move location  */
	FVector2D TouchDeltaMove = FVector2D(TouchLocation.X - Location.X, TouchLocation.Y - Location.Y);
	
	/** mind about sensitivity  */
	TouchDeltaMove *= TouchMouseSensivity;

	FVector NewLocation = GetActorLocation();
	/** find new location according to move limit  */
	NewLocation.X = FMath::Clamp(NewLocation.X + TouchDeltaMove.Y, -MoveLimit.Y, MoveLimit.Y);
	NewLocation.Y = FMath::Clamp(NewLocation.Y + TouchDeltaMove.X * -1.f, -MoveLimit.X, MoveLimit.X);

	/** this frame movement  */
	SetActorLocation(NewLocation);

	/** cache last frame location  */
	TouchLocation = FVector2D(Location.X, Location.Y);
}

void APlayerPawn::OnTouchPressed(ETouchIndex::Type Index, FVector Location)
{
	/** cache touch location  */
	TouchLocation = FVector2D(Location.X, Location.Y);
}

void APlayerPawn::ExplodePawn_Implementation()
{
	SetActorEnableCollision(false);

	ShootComponent->StopShooting();

	PawnMesh->SetMaterial(0, RecoverMaterial);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticles, GetActorTransform());
	
	/** deactivate all particles  */
	for (UActorComponent* PSC : GetComponentsByClass(UParticleSystemComponent::StaticClass()))
	{
		PSC->Deactivate();
	}
}

void APlayerPawn::RecoverPawn_Implementation()
{
	SetActorEnableCollision(true);

	ShootComponent->StartShooting();

	PawnMesh->SetMaterial(0, InitialMaterial);

	/** activate all particles  */
	for (UActorComponent* PSC : GetComponentsByClass(UParticleSystemComponent::StaticClass()))
	{
		PSC->Activate(true);
	}
}

