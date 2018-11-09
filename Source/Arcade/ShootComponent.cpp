// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "ShootComponent.h"
#include "ConstructorHelpers.h"
#include "Projectile.h"

void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
		
	StartShooting();
}

void UShootComponent::StartShooting()
{
	GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UShootComponent::Shoot, ShootPeriod, true);
}

void UShootComponent::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimer);
}

void UShootComponent::SetShootInfos(TArray<FShootInfo>& NewShootInfos, float NewShootPeriod /*= 0.2f*/)
{
	ShootInfos = NewShootInfos;
	ShootPeriod = NewShootPeriod;
}

void UShootComponent::Shoot()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		/** for each shoot info spawn specified projectile class and set its damage accordingly  */
		for (FShootInfo& ShootInfo : ShootInfos)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();
			if (GetOwner()->GetInstigator())
			{
				SpawnParams.Instigator = GetOwner()->GetInstigator();
			}
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(ShootInfo.Offset);

			FRotator SpawnRotation = GetOwner()->GetActorRotation();
			SpawnRotation.Add(0.f, ShootInfo.Angle, 0.f);

			if (ShootInfo.ProjectileClass)
			{
				AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ShootInfo.ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
				if (SpawnedProjectile)
				{
					SpawnedProjectile->SetDamage(ShootInfo.Damage);
				}
			}
		}
	}
}
