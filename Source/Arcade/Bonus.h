// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bonus.generated.h"

UCLASS()
class ARCADE_API ABonus : public AActor
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BonusMesh;
	
	/* sphere collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;
	
public:
	
	ABonus();

	/** pick up logic  */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** movement  */
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	/** called to play on collected logic and FX */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void BonusCollected(class APlayerPawn* PlayerPawn = nullptr);
	virtual void BonusCollected_Implementation(class APlayerPawn* PlayerPawn);

	/* on collected emitter template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* OnBonusCollectedEmitterTemplate;

};
