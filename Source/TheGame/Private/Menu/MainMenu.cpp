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
    AStaticData::GameOver = false;

    if (!ensure(Menu))
        return;

    UUserWidget* RunningMenu = nullptr;
    auto PreviousMenu = CurrentMenu;
    if (!AStaticData::Winner) {
        RunningMenu = Menu->NewGameStart();
    }
    else {
        RunningMenu = Menu->NewGameOver();
    }

    int32 Target = 0;
    RunningMenu->AddToViewport(Target);
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(
        GetWorld()->GetFirstPlayerController(),
        RunningMenu->GetWidgetFromName(TEXT("DefaultBtn")),
        EMouseLockMode::LockAlways
    );
}

AMainMenu::FMenus::FMenus(const ALevelScriptActor* Creator): Creator(Creator)
{
    GameStart = new ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/_Menus/GameStart"));
    GameOver  = new ConstructorHelpers::FClassFinder<UUserWidget>(TEXT("/Game/_Menus/GameOver"));
}

AMainMenu::FMenus::~FMenus()
{
    if (GameStart)
        delete GameStart;
    if (GameOver)
        delete GameOver;

    UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("menu destructor"));
}

UUserWidget* AMainMenu::FMenus::NewGameStart() const {
    return CreateWidget<UUserWidget>(Creator->GetWorld()->GetFirstPlayerController(), GameStart->Class);
}

UUserWidget* AMainMenu::FMenus::NewGameOver() const {
    return CreateWidget<UUserWidget>(Creator->GetWorld()->GetFirstPlayerController(), GameOver->Class);
}
