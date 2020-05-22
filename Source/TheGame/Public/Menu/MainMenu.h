// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenu.generated.h"

/**
 *   Level MainMenu inherits from this class
 *   This class is responsible for displaying the Menu UI Widgets
 */
UCLASS()
class THEGAME_API AMainMenu : public ALevelScriptActor
{
	GENERATED_BODY()
private:
    ConstructorHelpers::FClassFinder<UUserWidget>* MainMenu = nullptr;
protected:

    virtual ~AMainMenu();
    virtual void BeginPlay() override;
};
