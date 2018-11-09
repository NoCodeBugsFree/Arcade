// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "BonusShootLevelUp.h"
#include "ArcadeGameModeBase.h"

void ABonusShootLevelUp::BonusCollected_Implementation(class APlayerPawn* PlayerPawn)
{
	/** update shoot ability  */
	if(AArcadeGameModeBase* GM = Cast<AArcadeGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->ChangeShootLevel(true);
	}

	/** FX  */
	Super::BonusCollected_Implementation(PlayerPawn);
}
