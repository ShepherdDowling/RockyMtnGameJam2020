// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GodzillaAMI.generated.h"

/**
 *
 */
UCLASS(Transient, Blueprintable, BlueprintType)
class THEGAME_API UGodzillaAMI : public UAnimInstance
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        float Speed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
        bool bIsInAir = false;

    UGodzillaAMI();
    virtual ~UGodzillaAMI();
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
