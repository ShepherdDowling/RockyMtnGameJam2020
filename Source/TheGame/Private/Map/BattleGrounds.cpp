// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/BattleGrounds.h"
#include "Mode/DefaultGameMode.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"

ABattleGrounds::ABattleGrounds()
{
    //BpDefaultGameMode = new ConstructorHelpers::FClassFinder<AGameMode>(TEXT("/Game/_Levels/Modes/BpDefaultGameMode"));
}

ABattleGrounds::~ABattleGrounds()
{
    //if (BpDefaultGameMode)
    //    delete BpDefaultGameMode;
}

void ABattleGrounds::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("begin play battlegrounds"));
    GetWorld()->GetAuthGameMode<ADefaultGameMode>()->SetLevelLoaded(true);
    GetWorld()->GetAuthGameMode<ADefaultGameMode>()->Init();
    GetWorld()->GetAuthGameMode<ADefaultGameMode>()->SetLevelLoaded(false);
}

