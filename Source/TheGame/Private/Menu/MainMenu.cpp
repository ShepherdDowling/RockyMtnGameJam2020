// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MainMenu.h"

#include "Support/StaticData.h"

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
    Menu = new FMenus(this);
    ensure(Menu);
}

AMainMenu::~AMainMenu()
{
    if (Menu)
        delete Menu;
}

void AMainMenu::BeginPlay()
{
    Super::BeginPlay();

    if (!ensure(Menu))
        return;

    UUserWidget* RunningMenu = nullptr;
    if (!StaticData::Winner)
    {
         RunningMenu = Menu->NewGameStart();
    }
    else
    {
        RunningMenu = Menu->NewGameEnd();
    }

    int32 Target = 0;
    RunningMenu->AddToViewport(Target);
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
        GetWorld()->GetFirstPlayerController(),
        RunningMenu->GetWidgetFromName(TEXT("DefaultBtn")),
        EMouseLockMode::LockAlways
    );
    UWidgetBlueprintLibrary::SetFocusToGameViewport();
    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
    RunningMenu->RemoveFromRoot();
    
    StaticData::GameOver = false;
    StaticData::Winner = 0;
}

AMainMenu::FMenus::FMenus(const ALevelScriptActor* Creator): Creator(Creator)
{
    GameStart = new ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/_Menus/GameStart"));
    GameEnd   = new ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/_Menus/GameEnd"));
}

AMainMenu::FMenus::~FMenus()
{
    if (GameStart)
        delete GameStart;
    if (GameEnd)
        delete GameEnd;
}

UUserWidget* AMainMenu::FMenus::NewGameStart() const {
    return CreateWidget<UUserWidget>(Creator->GetWorld()->GetFirstPlayerController(), GameStart->Class);
}

UUserWidget* AMainMenu::FMenus::NewGameEnd() const {
    return CreateWidget<UUserWidget>(Creator->GetWorld()->GetFirstPlayerController(), GameEnd->Class);
}
