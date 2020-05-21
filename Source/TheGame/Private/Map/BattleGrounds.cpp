// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/BattleGrounds.h"
#include "Modes/DefaultGameMode.h"

#include "UObject/UObjectGlobals.h" 
#include "UObject/ConstructorHelpers.h" 
#include "Engine/World.h" 

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

#include "Engine/EngineBaseTypes.h" 
#include "Math/UnrealMathUtility.h" 

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
