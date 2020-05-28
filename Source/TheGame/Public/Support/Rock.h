// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"

#ifndef __CUSTOM_MODS__
#define __CUSTOM_MODS__

#define AnimatorBP ConstructorHelpers::FObjectFinder<UAnimSequence>

#define GetFirstPlayerPawn		GetWorld()->GetFirstPlayerController()->GetPawn()
#define GetFirstPlayerLocation	GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation()

#define GetFirstGodzilla		Cast<AGodzilla>(GetWorld()->GetFirstPlayerController()->GetPawn())

#endif


#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Rock.generated.h"


UCLASS()
class ARock : public AActor
{
	GENERATED_BODY()
public:

	static const FLinearColor Red;
	static const FLinearColor Green;
	static const FLinearColor Blue;

	static UPrimitiveComponent* GetActorComponent(const AActor* Actor, const FString& Name);
	static UPrimitiveComponent* GetActorComponent(const AActor* Actor, FString&& Name);

	static UPrimitiveComponent* GetActorComponentByRegex(const AActor* Actor, const FRegexPattern& Regex);
	static UPrimitiveComponent* GetActorComponentByRegex(const AActor* Actor, FRegexPattern&& Regex);
};
