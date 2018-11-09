// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ArcadeGameModeBase.h"
#include "EnemySpawnController.h"
#include "GameHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPawn.h"
#include "ConstructorHelpers.h"
#include "Projectile.h"

AArcadeGameModeBase::AArcadeGameModeBase()
{
	EnemySpawnController = CreateDefaultSubobject<UEnemySpawnController>(TEXT("EnemySpawnController"));

	GameHealthComponent = CreateDefaultSubobject<UGameHealthComponent>(TEXT("GameHealthComponent"));

	/** fill the shoot info levels data with some sensible default data */
	static ConstructorHelpers::FClassFinder<AProjectile> BPClass(TEXT("/Game/BP/BP_Projectile"));
	if (BPClass.Class)
	{
		// -----------------------------------------------------------------------------------
		/** level 01 - one projectile forward  */
		FShootInfoLevel ShootInfoLevel01;

		FShootInfo ShootInfo01_Level01;
		ShootInfo01_Level01.Offset = FVector(70.f, 0.f, 0.f);
		ShootInfo01_Level01.ProjectileClass = BPClass.Class;

		ShootInfoLevel01.ShootInfos.Add(ShootInfo01_Level01);
		// -----------------------------------------------------------------------------------

		// -----------------------------------------------------------------------------------
		/** level 02 - two projectiles forward */
		FShootInfoLevel ShootInfoLevel02;

		/** first projectile  */
		FShootInfo ShootInfo01_Level02;
		ShootInfo01_Level02.Offset = FVector(70.f, 20.f, 0.f);
		ShootInfo01_Level02.ProjectileClass = BPClass.Class;

		ShootInfoLevel02.ShootInfos.Add(ShootInfo01_Level02);

		/** second projectile  */
		FShootInfo ShootInfo02_Level02;
		ShootInfo02_Level02.Offset = FVector(70.f, -20.f, 0.f);
		ShootInfo02_Level02.ProjectileClass = BPClass.Class;

		ShootInfoLevel02.ShootInfos.Add(ShootInfo02_Level02);
		// -----------------------------------------------------------------------------------

		// -----------------------------------------------------------------------------------
		/** level 03 - two projectiles forward */
		FShootInfoLevel ShootInfoLevel03;

		/** first projectile  */
		FShootInfo ShootInfo01_Level03;
		ShootInfo01_Level03.Offset = FVector(70.f, 0.f, 0.f);
		ShootInfo01_Level03.ProjectileClass = BPClass.Class;

		ShootInfoLevel03.ShootInfos.Add(ShootInfo01_Level03);

		/** second projectile  */
		FShootInfo ShootInfo02_Level03;
		ShootInfo02_Level03.Offset = FVector(0.f, 50.f, 0.f);
		ShootInfo02_Level03.Angle = 5.f;
		ShootInfo02_Level03.ProjectileClass = BPClass.Class;

		ShootInfoLevel03.ShootInfos.Add(ShootInfo02_Level03);

		/** third projectile  */
		FShootInfo ShootInfo03_Level03;
		ShootInfo03_Level03.Offset = FVector(0.f, -50.f, 0.f);
		ShootInfo03_Level03.Angle = -5.f;
		ShootInfo03_Level03.ProjectileClass = BPClass.Class;

		ShootInfoLevel03.ShootInfos.Add(ShootInfo03_Level03);

		// -----------------------------------------------------------------------------------

		/** add levels  */
		ShootInfoLevels.Add(ShootInfoLevel01);
		ShootInfoLevels.Add(ShootInfoLevel02);
		ShootInfoLevels.Add(ShootInfoLevel03);
	}
}

void AArcadeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GameHealthComponent->OnHealthsEnded.AddDynamic(this, &AArcadeGameModeBase::EndGame);

	/** set player pawn reference  */
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(PlayerPawn)
	{
		PlayerPawn->OnPawnDamaged.AddDynamic(this, &AArcadeGameModeBase::ExplodePawn);

		/** set shoot info  */
		if (PlayerPawn->GetShootComponent() && ShootInfoLevels.IsValidIndex(CurrentShootLevel))
		{
			PlayerPawn->GetShootComponent()->SetShootInfos(ShootInfoLevels[0].ShootInfos);
		}
	}

	/** increase difficulty by timer  */
	FTimerHandle DifficultyTimer;
	GetWorldTimerManager().SetTimer(DifficultyTimer, this, &AArcadeGameModeBase::IncreaseDifficulty, IncreaseDifficultyDelay, true);
}

bool AArcadeGameModeBase::ChangeShootLevel(bool bUp /*= true*/)
{
	if (PlayerPawn)
	{
		/**  find new index  */
		int32 NewLevel = CurrentShootLevel + (bUp ? 1 : -1);
		NewLevel = FMath::Clamp(NewLevel, 0, ShootInfoLevels.Num() - 1);
		if (NewLevel == CurrentShootLevel)
		{
			/** same index = do nothing  */
			return false;
		}
		else
		{
			CurrentShootLevel = NewLevel;
		}

		/** set shoot info according new index  */
		if (PlayerPawn->GetShootComponent() && ShootInfoLevels.IsValidIndex(CurrentShootLevel))
		{
			PlayerPawn->GetShootComponent()->SetShootInfos(ShootInfoLevels[CurrentShootLevel].ShootInfos);
			return true;
		}
	}
	return false;
}

void AArcadeGameModeBase::IncreaseDifficulty()
{
	float CurrentMultiplier = EnemySpawnController->GetChangeStageTimeMultiplier();
	EnemySpawnController->SetChangeStageTimeMultiplier(FMath::Max(CurrentMultiplier * 0.95f, 0.4f));
}

void AArcadeGameModeBase::EndGame()
{
	bGameOver = true;

	EnemySpawnController->SetActive(false);

	OnGameOver.Broadcast();

	if (PlayerPawn)
	{
		PlayerPawn->Destroy();
	}

	UGameplayStatics::SetGamePaused(this, true);
}

void AArcadeGameModeBase::ExplodePawn_Implementation()
{
	if (PlayerPawn)
	{
		PlayerPawn->ExplodePawn();
		GameHealthComponent->ChangeHealths(-1);
		ChangeShootLevel(false);

		if (!bGameOver)
		{
			GetWorldTimerManager().SetTimer(RecoverTimer, this, &AArcadeGameModeBase::RecoverPawn, RecoverTime, false);
		}
	}
}

void AArcadeGameModeBase::RecoverPawn_Implementation()
{
	if (PlayerPawn)
	{
		PlayerPawn->RecoverPawn();
	}
}
