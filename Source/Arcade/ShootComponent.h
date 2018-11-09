// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootComponent.generated.h"

/** struct, describing current shoot ability  */
USTRUCT(BlueprintType, Blueprintable)
struct FShootInfo
{
	GENERATED_USTRUCT_BODY()

	/** projectile spawn offset according owner's actor location  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	FVector Offset;

	/** damage caused by this projectile  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float Damage = 20.f;

	/** angle to start moving, default zero is up direction  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float Angle = 0.f;

	/** projectile class template  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	TSubclassOf<class AProjectile> ProjectileClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADE_API UShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	/** called to start shooting timer */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StartShooting();

	/** called to deactivate shooting timer */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StopShooting();

	/** called to set new shoot info */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetShootInfos(TArray<FShootInfo>& NewShootInfos, float NewShootPeriod = 0.2f);

protected:
	
	/** starts shooting  */
	virtual void BeginPlay() override;

private:
	
	/** called to spawn projectile according current shoot info, set damage also 
		according shoot info
	*/
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Shoot();

	/** delay between shoots  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ShootPeriod = 0.2f;
	
	UPROPERTY()
	FTimerHandle ShootingTimer;
	
	/** list of shoot infos, describes all available shoot abilities for owner  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TArray<FShootInfo> ShootInfos;
	
};
