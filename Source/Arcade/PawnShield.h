// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PawnShield.generated.h"

UCLASS()
class ARCADE_API APawnShield : public AActor
{
	GENERATED_BODY()

	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;
	
public:	
	
	APawnShield();

	/** called to activate this shield on player (make him invulnerable) */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Activate(class APlayerPawn* PlayerPawn);
	
private:

	/** called to deactivate and destroy the shield (make owner vulnerable again) */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Deactivate();

	/** shiled life span  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DeactivationDelay = 10.f;

	/** shield's owner reference  */
	UPROPERTY(Transient)
	class APlayerPawn* Player;
	
};
