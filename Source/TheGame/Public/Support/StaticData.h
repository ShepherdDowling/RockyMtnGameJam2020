// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticData.generated.h"

/**
 *  This is a pod-class that holds data specific to this game
 *  Game & player data gets stored here that the GameMode will later reference. 
 *
 *  TODO: Update this to using an "AGameState" class
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEGAME_API AStaticData : public AActor
{
    GENERATED_BODY()
public:
    static bool GameOver; // = false
    static int  Winner;   // = 0; The user will think in terms of 1..

    UFUNCTION(BlueprintCallable, Category = "State")
    bool GetGameOver();

    UFUNCTION(BlueprintCallable, Category = "State")
    int GetWinner();
};
