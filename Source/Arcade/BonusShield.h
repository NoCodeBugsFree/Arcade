// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Bonus.h"
#include "BonusShield.generated.h"

/**
 * 
 */
UCLASS()
class ARCADE_API ABonusShield : public ABonus
{
	GENERATED_BODY()
	
public:

	ABonusShield();

protected:

	/** called to play on collected logic and FX */
	virtual void BonusCollected_Implementation(class APlayerPawn* PlayerPawn) override;
	
private:

	/** shield class template  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APawnShield> PawnShieldClass;

};
