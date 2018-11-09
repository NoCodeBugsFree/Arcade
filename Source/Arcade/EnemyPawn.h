// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawn.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FBonusChance
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	TSubclassOf<class ABonus> BonusClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA")
	float Chance = 0.5f;
};

UCLASS()
class ARCADE_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PawnMesh;

	/* box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxPawnCollision;

	/* shoot component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UShootComponent* ShootComponent;

	/* health component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

public:
	
	AEnemyPawn();

	/** movement  */
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

private:

	/** called to spawn bonus by chance after enemy is dead */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnBonuses();

	/** called to destroy enemy pawn  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DestroyPawn();

	/** called when player kills this guy to add some points, play FX and so on */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void KillPawn();

	/** called to  */
	UFUNCTION()
	void OnEnemyBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	/** points to killer which kills me  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Points = 10;

	/** list of bonuses and their chances to appear  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<FBonusChance> Bonuses;

	/** shows whether this enemy is homing to player pawn or not  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	uint32 bShouldMoveToPlayer : 1;

	/** player pawn reference  */
	UPROPERTY(Transient)
	class APawn* PlayerPawn;

	/** speed to rotate this guy according player pawn current location  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float RotMaxSpeed = 60.f;

	/* explosion emitter template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ExplodeParticles;
};
