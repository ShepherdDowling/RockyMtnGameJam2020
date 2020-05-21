// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BattleGrounds.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API ABattleGrounds : public ALevelScriptActor
{
	GENERATED_BODY()
private:
    //ConstructorHelpers::FClassFinder<class AGameMode>* BpDefaultGameMode = nullptr;
protected:

public:

    ABattleGrounds();
    virtual ~ABattleGrounds();
    virtual void BeginPlay() override;
};
