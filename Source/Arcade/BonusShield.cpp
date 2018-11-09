// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "BonusShield.h"
#include "ConstructorHelpers.h"
#include "PawnShield.h"
#include "PlayerPawn.h"

ABonusShield::ABonusShield()
{
	/**  set the shield class */
	static ConstructorHelpers::FClassFinder<APawnShield> BPClass(TEXT("/Game/BP/BP_PawnShield"));
	if (BPClass.Class)
	{
		PawnShieldClass = BPClass.Class;
	}
}

void ABonusShield::BonusCollected_Implementation(APlayerPawn* PlayerPawn)
{
	if (PlayerPawn && PlayerPawn->bCanBeDamaged)
	{
		if (PawnShieldClass)
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator; //  GetController();
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				
				FVector SpawnLocation = GetActorLocation();
				FRotator SpawnRotation = GetActorRotation();
				
				/** spawn and activate the shield  */
				APawnShield* SpawnedPawnShield = World->SpawnActor<APawnShield>(PawnShieldClass, SpawnLocation, SpawnRotation, SpawnParams);
				if (SpawnedPawnShield)
				{
					SpawnedPawnShield->Activate(PlayerPawn);
				}
			}
		}
	}

	/** FX  */
	Super::BonusCollected_Implementation(PlayerPawn);
}
