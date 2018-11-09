// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "EnemySpawnController.h"

void UEnemySpawnController::BeginPlay()
{
	Super::BeginPlay();
	
	RandomStream.GenerateNewSeed();

	StartSpawnStage();
}

void UEnemySpawnController::Deactivate()
{
	Super::Deactivate();
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UEnemySpawnController::StartSpawnStage()
{
	if (SpawnStages.Num() > 0)
	{
		/** spawn random of all available  */
		SpawnStage = SpawnStages[RandomStream.RandRange(0, SpawnStages.Num() - 1)];
		EnemiesSpawned = 0;
		SpawnEnemy();
		
		float ChangeStageTime = RandomStream.FRandRange(StageDelayMin, StageDelayMax);

		ChangeStageTime *= ChangeStageTimeMultiplier;

		GetWorld()->GetTimerManager().SetTimer(ChangeStageTimer, this, &UEnemySpawnController::StartSpawnStage, ChangeStageTime, false);
	}
}

void UEnemySpawnController::SetChangeStageTimeMultiplier(float NewChangeStageTimeMultiplier)
{
	ChangeStageTimeMultiplier = NewChangeStageTimeMultiplier;
}

void UEnemySpawnController::SpawnEnemy()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetOwner()->GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
		FVector SpawnLocation;
		FRotator SpawnRotation;
		FTransform SpawnTransform;
		
		if (SpawnStage.EnemyClass)
		{
			/** spawn an enemy  */
			AEnemyPawn* Spawned = World->SpawnActor<AEnemyPawn>(SpawnStage.EnemyClass, SpawnStage.SpawnTransform, SpawnParams);
			if (Spawned)
			{
				EnemiesSpawned++;

				/** spawn more by timer  */
				if (EnemiesSpawned < SpawnStage.NumOfEnemies)
				{
					GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimer, this, &UEnemySpawnController::SpawnEnemy, SpawnStage.SpawnDelay, false);
				}
			}
		}
	}	
}
