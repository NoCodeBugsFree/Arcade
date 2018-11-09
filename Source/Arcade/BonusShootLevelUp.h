// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Bonus.h"
#include "BonusShootLevelUp.generated.h"

/**
 * 
 */
UCLASS()
class ARCADE_API ABonusShootLevelUp : public ABonus
{
	GENERATED_BODY()
	
protected:
	/** called to play on collected logic and FX */
	virtual void BonusCollected_Implementation(class APlayerPawn* PlayerPawn) override;

};
