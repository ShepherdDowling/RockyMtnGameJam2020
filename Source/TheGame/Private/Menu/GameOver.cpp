// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu/GameOver.h"
#include "Support/StaticData.h"

FString UGameOver::GetWinner() const
{
    if (AStaticData::Winner == 1)
        return FString("Player 1 Wins!!");
    else
        return FString("Player 2 Wins!!");
}

void UGameOver::RemoveWinner() const
{
    AStaticData::Winner = 0;
}
