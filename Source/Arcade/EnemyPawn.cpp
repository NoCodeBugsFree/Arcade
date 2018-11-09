// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "EnemyPawn.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "ShootComponent.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ArcadeGameModeBase.h"
#include "Bonus.h"
#include "ConstructorHelpers.h"
#include "BonusPoints.h"
#include "BonusShield.h"
#include "BonusShootLevelUp.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystem.h"

AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* box collision  */
	BoxPawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxPawnCollision);
	BoxPawnCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxPawnCollision->SetCollisionObjectType(ECC_Pawn);

	/** mesh  */
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	PawnMesh->SetupAttachment(RootComponent);
	PawnMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/**  set the default static mesh */
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (Cube.Succeeded())
	{
		PawnMesh->SetStaticMesh(Cube.Object);
	}

	/* shoot component  */
	ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("ShootComponent"));

	/* health component  */
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	/** fill bonuses array  */
	FBonusChance Bonus01;
	
	static ConstructorHelpers::FClassFinder<ABonusPoints> BP_BonusPointsClass(TEXT("/Game/BP/Bonuses/BP_BonusPoints"));
	if (BP_BonusPointsClass.Class != NULL)
	{
		Bonus01.BonusClass = BP_BonusPointsClass.Class;
		Bonus01.Chance = 30.f;
	}
	
	FBonusChance Bonus02;

	static ConstructorHelpers::FClassFinder<ABonusPoints> BonusPointsBigClass(TEXT("/Game/BP/Bonuses/BP_BonusPointsBig"));
	if (BonusPointsBigClass.Class != NULL)
	{
		Bonus02.BonusClass = BonusPointsBigClass.Class;
		Bonus02.Chance = 20.f;
	}

	FBonusChance Bonus03;

	static ConstructorHelpers::FClassFinder<ABonusShield> BonusShieldClass(TEXT("/Game/BP/BP_BonusShield"));
	if (BonusShieldClass.Class != NULL)
	{
		Bonus03.BonusClass = BonusShieldClass.Class;
		Bonus03.Chance = 20.f;
	}

	FBonusChance Bonus04;

	static ConstructorHelpers::FClassFinder<ABonusShootLevelUp> BonusShootLevelUpClass(TEXT("/Game/BP/BP_BonusShootLevelUp"));
	if (BonusShootLevelUpClass.Class != NULL)
	{
		Bonus04.BonusClass = BonusShootLevelUpClass.Class;
		Bonus04.Chance = 20.f;
	}

	Bonuses.Add(Bonus01);
	Bonuses.Add(Bonus02);
	Bonuses.Add(Bonus03);
	Bonuses.Add(Bonus04);

	/** set the explosion emitter  */
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem(TEXT("/Game/Assest/Particles/P_Explode"));
	if (ParticleSystem.Object)
	{
		ExplodeParticles = ParticleSystem.Object;
	}
}

void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnHealthEnded.AddDynamic(this, &AEnemyPawn::KillPawn);

	OnActorBeginOverlap.AddDynamic(this, &AEnemyPawn::OnEnemyBeginOverlap);

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/** simple move-down logic  */
	float WorldMoveOffset = -100.f;
	WorldMoveOffset *= DeltaTime;
	AddActorWorldOffset(FVector(WorldMoveOffset, 0.f, 0.f));

	/** player homing logics  */
	if (bShouldMoveToPlayer && PlayerPawn)
	{
		AddActorWorldOffset(GetActorForwardVector() * 4.f);
		FVector EnemyLocation = GetActorLocation();
		FVector PlayerPawnLocation = PlayerPawn->GetActorLocation();
		
		FRotator FindLookRotator = FRotationMatrix::MakeFromX(PlayerPawnLocation - EnemyLocation).Rotator();
		FRotator Delta = FindLookRotator - GetActorRotation();
		Delta.Normalize();
		float Yaw = FMath::Clamp(Delta.Yaw, -RotMaxSpeed, RotMaxSpeed);
		Yaw *= DeltaTime;
		AddActorLocalRotation(FRotator(0.f, Yaw, 0.f)); 
	}
}

void AEnemyPawn::SpawnBonuses()
{
	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
						
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	
	/** try to spawn bonus if we have some luck  */
	for (const FBonusChance& Bonus : Bonuses)
	{
		float RandomFloat = RandomStream.FRandRange(0.f, 100.f);
		
		if (RandomFloat < Bonus.Chance)
		{
			if (Bonus.BonusClass)
			{
				UWorld* const World = GetWorld();
				if (World)
				{
					ABonus* Spawned = World->SpawnActor<ABonus>(Bonus.BonusClass, SpawnLocation, SpawnRotation, SpawnParams);
					if (Spawned)
					{
						/** one bonus per enemy  */
						break;
					}
				}
			}
		}
	}
}

void AEnemyPawn::DestroyPawn()
{
	Destroy();
}

void AEnemyPawn::KillPawn()
{
	/** add some points to killers score  */
	if (AArcadeGameModeBase* GM = Cast<AArcadeGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->AddPoints(Points);
	}

	/** spawn bonuses if we have some luck  */
	SpawnBonuses();

	/** FX  */
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticles, GetActorTransform());

	DestroyPawn();
}

void AEnemyPawn::OnEnemyBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	/** damage the player on touch  */
	if (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0) && !OverlappedActor->IsPendingKill())
	{
		/** damage the player  */
		const float Damage = UGameplayStatics::ApplyDamage(OtherActor, 100.f, GetInstigatorController(), this, UDamageType::StaticClass());

		if (Damage > 0.f)
		{
			/** destroy self  */
			DestroyPawn();
		}
	}
}


