// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MenuSystem.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API AMenuSystem : public ALevelScriptActor
{
	GENERATED_BODY()
private:
    ConstructorHelpers::FClassFinder<UUserWidget>* MainMenu = nullptr;
protected:

    virtual ~AMenuSystem();
    virtual void BeginPlay() override;
};
