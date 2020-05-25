// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MainMenu.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "Asset/DefaultHUD.h"

#include "UObject/UObjectGlobals.h" 
#include "UObject/ConstructorHelpers.h" 

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Controller.h"

#include "Engine/World.h" 
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h" 


AMainMenu::AMainMenu()
{
    CHMainMenu = new ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/_Menus/MainMenuUI"));
    ensure(CHMainMenu);
}

AMainMenu::~AMainMenu()
{
    if (CHMainMenu)
        delete CHMainMenu;
}

void AMainMenu::BeginPlay()
{
    Super::BeginPlay();
    MainMenu = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), CHMainMenu->Class);

    if (!ensure(MainMenu))
        return;

    int32 Target = 0;
    MainMenu->AddToViewport(Target);
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
        GetWorld()->GetFirstPlayerController(), 
        MainMenu->GetWidgetFromName(TEXT("StartBtn")), 
        EMouseLockMode::LockAlways
    );
    UWidgetBlueprintLibrary::SetFocusToGameViewport();
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
}
