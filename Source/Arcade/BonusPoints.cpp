// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "BonusPoints.h"
#include "ArcadeGameModeBase.h"

void ABonusPoints::BonusCollected_Implementation(APlayerPawn* PlayerPawn)
{
	/** add points  */
	if(AArcadeGameModeBase* GM = Cast<AArcadeGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->AddPoints(Points);
	}

	/** FX  */
	Super::BonusCollected_Implementation(PlayerPawn);
}
