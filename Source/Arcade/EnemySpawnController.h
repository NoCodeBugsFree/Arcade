// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyPawn.h"
#include "EnemySpawnController.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FEnemySpawnInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AEnemyPawn> EnemyClass = AEnemyPawn::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NumOfEnemies = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelay = 1.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADE_API UEnemySpawnController : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	/** clear all timers for this object  */
	virtual void Deactivate() override;

	/** setter to ChangeStageTimeMultiplier */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetChangeStageTimeMultiplier(float NewChangeStageTimeMultiplier);

protected:
	
	/** start spawning  */
	virtual void BeginPlay() override;
	
private:

	/** called to spawn an enemy */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnEnemy();

	/** called to  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StartSpawnStage();

	/** spawn infos for this game  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<FEnemySpawnInfo> SpawnStages;

	/** current spawn info  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FEnemySpawnInfo SpawnStage;

	/** min delay between spawn  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float StageDelayMin = 1.f;

	/** max delay between spawn  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float StageDelayMax = 2.f;
	
	UPROPERTY()
	FTimerHandle ChangeStageTimer;
	
	UPROPERTY()
	FTimerHandle EnemySpawnTimer;

	/** current spawned enemies amount */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 EnemiesSpawned = 0;

	UPROPERTY()
	FRandomStream RandomStream;

	/** change stage multiplier  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ChangeStageTimeMultiplier = 1.f;

public:	
	FORCEINLINE float GetChangeStageTimeMultiplier() const { return ChangeStageTimeMultiplier; }
};
