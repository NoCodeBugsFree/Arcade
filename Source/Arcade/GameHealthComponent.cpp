// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "GameHealthComponent.h"
#include "Kismet/GameplayStatics.h"

void UGameHealthComponent::ChangeHealths(int32 Value)
{
	/** update health  */
	Healths += Value;

	/** broadcast about accordingly */
	OnHealthsChanged.Broadcast(Value);

	if (Healths <= 0)
	{
		OnHealthsEnded.Broadcast();
	}
}
