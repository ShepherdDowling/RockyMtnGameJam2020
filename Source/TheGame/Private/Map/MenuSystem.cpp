// Fill out your copyright notice in the Description page of Project Settings.

#include "Map/MenuSystem.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "Widget/DefaultUI.h"
#include "UObject/UObjectGlobals.h" 
#include "Engine/World.h" 
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


AMenuSystem::~AMenuSystem()
{
    if (MainMenu)
        delete MainMenu;
}

void AMenuSystem::BeginPlay()
{
    
    //MainMenu = new ConstructorHelpers::FClassFinder<UUserWidget>("/Game/_Levels/Menus/MainMenuUI");
    //ensure(MainMenu);

}
