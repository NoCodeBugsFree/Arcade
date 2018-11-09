// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArcadeHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARCADE_API AArcadeHUD : public AHUD
{
	GENERATED_BODY()
	
	/** represents player life */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UTexture* PlayerLifeTexture;

	/** texture */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UTexture* PauseTexture;

	/** font */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UFont* Font;
	
public:

	AArcadeHUD();

	/** sets GM reference  */
	virtual void BeginPlay() override;

	/** The Main Draw loop for the hud.  Gets called before any messaging.  Should be subclassed */
	virtual void DrawHUD() override;

	/** Native handler, called when a hit box is clicked on. Provides the name associated with that box. */
	virtual void NotifyHitBoxClick(FName BoxName) override;
	
private:

	/** called to draw healths */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DrawHealth();

	/** called to draw pause button and hotbox to activate it */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DrawPause();

	/** called to draw player points */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void DrawPoints();

	/** called to apply anchor to given coordinates */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ApplyCordsAncors(FVector2D& In, FVector2D Anchor) const;

	/** called to scale 2D coordinates accordingly current viewport scale */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ScaleCoordinates(FVector2D& In) const;

	/** called to create game over widget menu */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void GameOver();
	
	/** called to create game over widget menu */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void CreateGameOverWidget();

	/** called to create game over widget menu */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void CreatePauseMenuWidget();

	/** GM reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class AArcadeGameModeBase* GM;

	/** health component reference  */
	UPROPERTY(BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	class UGameHealthComponent* GameHealthComponent;

	/** the name of pause hit box name to catch in NotifyHitBoxClick  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FName PauseHitBoxName = FName(TEXT("Pause"));

	/** game over widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> GameOverWidgetClass;

	/** pause menu widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;
	
	/** current active game over widget */
	UPROPERTY()
	class UUserWidget* CurrentGameOverWidget;

	/** current active pause widget */
	UPROPERTY()
	class UUserWidget* CurrentPauseWidget;
};
