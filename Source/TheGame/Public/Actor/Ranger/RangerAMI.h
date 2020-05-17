// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RangerAMI.generated.h"

/**
 * 
 */
UCLASS(Transient, Blueprintable, BlueprintType)
class THEGAME_API URangerAMI : public UAnimInstance
{
	GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bIsInAir = false;
    
    URangerAMI();
    virtual ~URangerAMI();
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
