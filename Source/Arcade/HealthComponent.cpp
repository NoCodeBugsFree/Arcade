// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnOwnerDamaged);
	}
}

void UHealthComponent::OnOwnerDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	ChangeHealth(-Damage);
}

void UHealthComponent::ChangeHealth(float Amount)
{
	/** update health  */
	Health += Amount;

	if (Health <= 0.f)
	/** we are dead - broadcast AB  */
	{
		if (GetOwner())
		{
			GetOwner()->OnTakeAnyDamage.RemoveAll(this);

			OnHealthEnded.Broadcast();
		}
	}
}

