// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenu.generated.h"

class UUserWidget;
class UWidgetBlueprintLibrary;

/**
 *   Level MainMenu inherits from this class
 *   This class is responsible for displaying the Menu UI Widgets
 */
UCLASS()
class THEGAME_API AMainMenu : public ALevelScriptActor
{
	GENERATED_BODY()
private:
    struct FMenus
    {
        const ALevelScriptActor* Creator = nullptr;
        ConstructorHelpers::FClassFinder<UUserWidget>* GameStart = nullptr;
        ConstructorHelpers::FClassFinder<UUserWidget>* GameOver  = nullptr;
        FMenus(const ALevelScriptActor* Creator);
        ~FMenus();
        UUserWidget* NewGameStart() const;
        UUserWidget* NewGameOver() const;
    };

    enum class EMenu
    {
        None,
        GameStart,
        GameOver
    };
    EMenu CurrentMenu = EMenu::None;

    FMenus* Menu = nullptr;

protected:
    AMainMenu();
    virtual ~AMainMenu();
    virtual void BeginPlay() override;
};
