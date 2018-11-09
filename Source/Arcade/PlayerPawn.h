// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPawnDamaged);

UCLASS()
class ARCADE_API APlayerPawn : public APawn
{
	GENERATED_BODY()
	
	/** static mesh component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PawnMesh;

	/* box collision  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxPawnCollision;
	
	/** camera boom  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* camera component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/* shoot component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UShootComponent* ShootComponent;

	/* health component  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	/** player's initial material  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* InitialMaterial;

	/** player's blink material  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UMaterialInterface* RecoverMaterial;

	/* engine particles  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* EngineFireParcticles;
	
public:

	APlayerPawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** called to take damage if we are vulnerable  */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/** broadcasts whenever owner's pawn takes damage  */
	UPROPERTY(BlueprintAssignable, Category = "EventDispatcher")
	FOnPawnDamaged OnPawnDamaged;

	/** called whenever pawn is exploded, disabling it temporarily */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void ExplodePawn();
	virtual void ExplodePawn_Implementation();

	/** called whenever pawn is activated after death, enabling all its functionality */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void RecoverPawn();
	virtual void RecoverPawn_Implementation();

protected:

	/** some delegates bindings  */
	virtual void BeginPlay() override;
	
	/** set PC reference  */
	virtual void PossessedBy(AController* NewController) override;
	
private:
	
	/** called to handle touch move event, moves player to touch move location  */
	void OnTouchMove(ETouchIndex::Type Index, FVector Location);

	/** called to set touch location  */
	void OnTouchPressed(ETouchIndex::Type Index, FVector Location);
	
	// -----------------------------------------------------------------------------------

	/** PC reference  */
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class APlayerController* PC;

	/** last touch location  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector2D TouchLocation;

	/** touch mouse sensitivity  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float TouchMouseSensivity = 1.f;

	/** box limiting our movement to prevent off the board movement  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FVector2D MoveLimit = FVector2D(500.f, 600.f);

	/* explode emitter template */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ExplodeParticles;
	
public:	
	/** returns shoot component **/
	FORCEINLINE class UShootComponent* GetShootComponent() const { return ShootComponent; }
};
