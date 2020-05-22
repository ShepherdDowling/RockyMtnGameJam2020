// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MainMenu.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "Asset/DefaultHUD.h"
#include "UObject/UObjectGlobals.h" 
#include "Engine/World.h" 
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


AMainMenu::~AMainMenu()
{
    if (MainMenu)
        delete MainMenu;
}

void AMainMenu::BeginPlay()
{
    //MainMenu = new ConstructorHelpers::FClassFinder<UUserWidget>("/Game/_Menus/MainMenuUI");
    //ensure(MainMenu);
}
