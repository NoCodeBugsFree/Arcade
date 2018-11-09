// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "PlaygroundBorder.h"
#include "Components/BoxComponent.h"
#include "PlayerPawn.h"

APlaygroundBorder::APlaygroundBorder()
{
	/* box collision  */
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetCollisionProfileName("OverlapAll");
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetBoxExtent(FVector(2000.f, 2000.f, 2000.f));
	SetRootComponent(Trigger);
}

void APlaygroundBorder::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if ( (OtherActor) && (OtherActor != this) && ( ! OtherActor->IsPendingKill() ) )
	{
		if(APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor))
		{
			// ignore player pawn
		}
		else /** destroy anything  */
		{
			OtherActor->Destroy();
		}
	}
}
