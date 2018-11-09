// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Bonus.h"
#include "BonusPoints.generated.h"

/**
 * This bonus adds some extra points.
 */
UCLASS()
class ARCADE_API ABonusPoints : public ABonus
{
	GENERATED_BODY()
	
protected:

	/** called to play on collected logic and FX */
	virtual void BonusCollected_Implementation(class APlayerPawn* PlayerPawn) override;

private:

	/** points to add  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Points = 50;
	
};
