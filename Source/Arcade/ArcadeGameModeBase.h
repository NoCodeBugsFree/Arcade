// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootComponent.h"
#include "ArcadeGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

/** structs, representing list of shoot infos and shoot period for each instance  */
USTRUCT(BlueprintType, Blueprintable)
struct FShootInfoLevel
{
	GENERATED_USTRUCT_BODY()

	/** list of shoot infos  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	TArray<FShootInfo> ShootInfos;

	/** delay between shots  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	float ShootPeriod = 0.2;
};

/**
 * 
 */
UCLASS()
class ARCADE_API AArcadeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	/** enemy spawn component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UEnemySpawnController* EnemySpawnController;

	/** game health component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UGameHealthComponent* GameHealthComponent;
	
public: 

	AArcadeGameModeBase();

	/** game over event dispatcher  */
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnGameOver OnGameOver;

	/** called to add some points */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddPoints(int32 Points) { GamePoints += Points; }

	/** called to up/down current shot level */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool ChangeShootLevel(bool bUp = true);
	
	/** called to increase game difficulty */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void IncreaseDifficulty();

protected:

	virtual void BeginPlay() override;

	/** called whenever player pawn was exploded */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void ExplodePawn();
	virtual void ExplodePawn_Implementation();

	/** called whenever player pawn was exploded */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void RecoverPawn();
	virtual void RecoverPawn_Implementation();

private:

	/** called to finish the game */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void EndGame();
	
	// -----------------------------------------------------------------------------------

	/** time to recover player pawn after death  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float RecoverTime = 3.f;
	
	UPROPERTY()
	FTimerHandle RecoverTimer;

	/** player pawn reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class APlayerPawn* PlayerPawn;

	/** shows whether the game is over or not  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bGameOver : 1;

	/** player's current score  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 GamePoints = 0;

	/** defines current shoot abilities according current shoot level */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<FShootInfoLevel> ShootInfoLevels;

	/** show how cool we fire right now  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 CurrentShootLevel = 0;

	/** how fast the difficulty of the game have been increased  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float IncreaseDifficultyDelay = 10.f;

public:
	FORCEINLINE int32 GetGamePoints() const { return GamePoints; }
	FORCEINLINE class UGameHealthComponent* GetHealthComp() const { return GameHealthComponent; }
	FORCEINLINE int32 GetCurrentShootLevel() const { return CurrentShootLevel; }
};
