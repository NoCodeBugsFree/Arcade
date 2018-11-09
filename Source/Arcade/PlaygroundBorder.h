// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaygroundBorder.generated.h"

UCLASS()
class ARCADE_API APlaygroundBorder : public AActor
{
	GENERATED_BODY()

	/* box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Trigger;
		
public:
	
	APlaygroundBorder();

	/** garbage collector =)  */
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
private:
	
};
