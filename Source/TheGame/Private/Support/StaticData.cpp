// Fill out your copyright notice in the Description page of Project Settings.


#include "Support/StaticData.h"

bool AStaticData::GameOver = false;
int  AStaticData::Winner = 0;

bool AStaticData::GetGameOver()
{
    return AStaticData::GameOver;
}

int AStaticData::GetWinner()
{
    return AStaticData::Winner;
}
