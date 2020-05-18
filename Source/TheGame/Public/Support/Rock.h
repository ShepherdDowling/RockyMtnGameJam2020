// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"

#ifndef __CUSTOM_MODS__
#define __CUSTOM_MODS__

typedef UGameplayStatics Statics;

#define GetFirstPlayerPawn GetWorld()->GetFirstPlayerController()->GetPawn()
#define GetFirstGodzilla Cast<AGodzilla>(GetWorld()->GetFirstPlayerController()->GetPawn())
#define GetFirstPlayerLocation GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation()

#define AnimatorBP ConstructorHelpers::FObjectFinder<UAnimSequence>

#endif

// mc overlaps cc
// mc will be used where cc can't is when there is
// non-unreal engine inheritence in use.

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Rock.generated.h"


UCLASS()
class ARock : public AActor
{
	GENERATED_BODY()
	
private:	

protected:
	//virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ARock();
	//virtual void Tick(float DeltaTime) override;
};

