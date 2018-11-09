// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthEnded);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	/** called to increase/decrease the health by Amount
		@param Amount can be positive or negative
	*/
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ChangeHealth(float Amount);

	/** health ended delegate  */
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnHealthEnded OnHealthEnded;

protected:
	
	/** on owner take damage delegate binging  */
	virtual void BeginPlay() override;

private:

	/** called when owner was damaged  */
	UFUNCTION()
	void OnOwnerDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/** health amount  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

public:
	/** returns health amount **/
	FORCEINLINE float GetHealth() const { return Health; }
	
};
