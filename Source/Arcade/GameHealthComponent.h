// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthsEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthsChanged, int32, ChangeValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCADE_API UGameHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	/** called to change owner's health by Value  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ChangeHealths(int32 Value);

	/** healths ended delegate  */
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnHealthsEnded OnHealthsEnded;

	/** healths changed delegate  */
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnHealthsChanged OnHealthsChanged;

private:

	/** current health (or lives) amount  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	int32 Healths = 3;

public:	
	FORCEINLINE int32 GetHealths() const { return Healths; }
	
};
