// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <atomic>

/**
 *  This is a pod-class that holds data specific to this game
 *  Game & player data gets stored here that the GameMode will later reference. 
 */
class StaticData
{
public:
    std::atomic<bool> PlayerDied = false;
};