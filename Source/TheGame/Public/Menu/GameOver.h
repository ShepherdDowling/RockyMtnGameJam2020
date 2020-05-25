// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOver.generated.h"

/**
 * 
 */
UCLASS()
class THEGAME_API UGameOver : public UUserWidget
{
	GENERATED_BODY()
protected:
    UFUNCTION(BlueprintCallable, Category = "Method")
    FString GetWinner() const;

    UFUNCTION(BlueprintCallable, Category = "Method")
    void RemoveWinner() const;
};
