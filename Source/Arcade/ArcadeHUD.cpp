// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcadeHUD.h"
#include "ArcadeGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameHealthComponent.h"
#include "Engine/Font.h"
#include "ConstructorHelpers.h"

AArcadeHUD::AArcadeHUD()
{
	/**  set the game over widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverClassBPClass(TEXT("/Game/BP/UMG/WBP_GameOver"));
	if (GameOverClassBPClass.Class)
	{
		GameOverWidgetClass = GameOverClassBPClass.Class;
	}

	/**  set the pause menu widget asset */
	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuClassBPClass(TEXT("/Game/BP/UMG/WBP_PauseMenu"));
	if (PauseMenuClassBPClass.Class)
	{
		PauseMenuWidgetClass = PauseMenuClassBPClass.Class;
	}

	/** set player life texture  */
	static ConstructorHelpers::FObjectFinder<UTexture> PlayerLifeTextureTemplate(TEXT("/Engine/EngineResources/AICON-Green"));
	if (PlayerLifeTextureTemplate.Succeeded())
	{
		PlayerLifeTexture = PlayerLifeTextureTemplate.Object;
	}

	/** set pause button texture asset  */
	static ConstructorHelpers::FObjectFinder<UTexture> TextureTemplate(TEXT("/Engine/EditorResources/Ai_Spawnpoint"));
	if (TextureTemplate.Succeeded())
	{
		PauseTexture = TextureTemplate.Object;
	}
	
	/** set font asset  */
	static ConstructorHelpers::FObjectFinder<UFont> FontTemplate(TEXT("/Game/BP/UMG/Fonts/ArcadeFont"));
	if (FontTemplate.Succeeded())
	{
		Font = FontTemplate.Object;
	}
}

void AArcadeHUD::BeginPlay()
{
	Super::BeginPlay();

	/** set GM ref  */
	if(AArcadeGameModeBase* TestGM = Cast<AArcadeGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM = TestGM;
		GM->OnGameOver.AddDynamic(this, &AArcadeHUD::GameOver);
		if (GM->GetHealthComp())
		{
			GameHealthComponent = GM->GetHealthComp();
		}
	}
}

void AArcadeHUD::DrawHUD()
{
	Super::DrawHUD();

	/** health aka lives  */
	DrawHealth();

	/** pause  */
	DrawPause();

	/** score  */
	DrawPoints();
}

void AArcadeHUD::NotifyHitBoxClick(FName BoxName)
{
	Super::NotifyHitBoxClick(BoxName);

	/** pause button clicked */
	if (BoxName == PauseHitBoxName)
	{
		UGameplayStatics::SetGamePaused(this, true);
		CreatePauseMenuWidget();
	}
}

void AArcadeHUD::DrawHealth()
{
	if (GameHealthComponent)
	{
		/** for each live draw texture  */
		for (int Index = 0; Index < GameHealthComponent->GetHealths() - 1; Index++)
		{
			float ToScaleX = Index * (64.f + 10.f) + 20.f;
			FVector2D ToScale2D_XY = FVector2D(ToScaleX, 20.f);
			FVector2D ToScale2D_WH = FVector2D(64.f, 64.f);

			/** draw texture */
			if (PlayerLifeTexture)
			{
				DrawTextureSimple(PlayerLifeTexture, ToScale2D_XY.X, ToScale2D_XY.Y);
			} 
			else
			{
				UE_LOG(LogTemp, Error, TEXT("PlayerLifeTexture is null"));
			}
		}
	}
}

void AArcadeHUD::DrawPause()
{
	FVector2D ToScale_XY = FVector2D(-84.f, 20.f);
	ScaleCoordinates(ToScale_XY);
	ApplyCordsAncors(ToScale_XY, FVector2D(1.f, 0.f));
	FVector2D ToScale2D_WH = FVector2D(64.f, 64.f);
	ScaleCoordinates(ToScale2D_WH);

	/** draw texture */
	if (PauseTexture)
	{
		DrawTexture(PauseTexture, ToScale_XY.X, ToScale_XY.Y, ToScale2D_WH.X, ToScale2D_WH.Y, 0.f, 0.f, 1.f, 1.f);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PauseTexture is null"));
	}

	/** pauses hit box at pause texture location */
	AddHitBox(ToScale_XY, ToScale2D_WH, PauseHitBoxName, false);
}

void AArcadeHUD::DrawPoints()
{
	FString TextToDraw = "Points: ";
	if (GM)
	{
		int32 Points = GM->GetGamePoints();
		TextToDraw += FString::FromInt(Points);
		float OutWidth, OutHight;
		GetTextSize(FString::FromInt(Points), OutWidth, OutHight, Font, 1.f);
		OutWidth *= -1.f;

		FVector2D ToScale = FVector2D(OutWidth, 20.f);
		ScaleCoordinates(ToScale);
		ApplyCordsAncors(ToScale, FVector2D(0.5f, 0.f));

		/** draw points text  */
		DrawText(TextToDraw, FLinearColor::Red, ToScale.X, ToScale.Y, Font);
	}
}

void AArcadeHUD::ApplyCordsAncors(FVector2D& In, FVector2D Anchor) const
{
	In += UWidgetLayoutLibrary::GetViewportSize(PlayerOwner) * Anchor;
}

void AArcadeHUD::ScaleCoordinates(FVector2D& In) const
{
	In *= UWidgetLayoutLibrary::GetViewportScale(PlayerOwner);
}

void AArcadeHUD::GameOver()
{
	CreateGameOverWidget();
}

void AArcadeHUD::CreateGameOverWidget()
{
	if (GameOverWidgetClass)
	{
		CurrentGameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
		if (CurrentGameOverWidget)
		{
			CurrentGameOverWidget->AddToViewport();
			if (PlayerOwner)
			{
				PlayerOwner->bShowMouseCursor = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameOverWidgetClass is null!"));
	}
}

void AArcadeHUD::CreatePauseMenuWidget()
{
	if (PauseMenuWidgetClass)
	{
		CurrentPauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
		if (CurrentPauseWidget)
		{
			CurrentPauseWidget->AddToViewport();
			if (PlayerOwner)
			{
				PlayerOwner->bShowMouseCursor = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PauseMenuWidgetClass is null!"));
	}
}