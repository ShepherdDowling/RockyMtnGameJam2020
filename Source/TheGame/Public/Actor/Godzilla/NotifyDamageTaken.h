// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyDamageTaken.generated.h"



/**
 * 
 */
UCLASS()
class THEGAME_API UNotifyDamageTaken : public UAnimNotifyState
{
	GENERATED_BODY()
private:
    struct FBone
    {
        FName Start;
        FName End;
    };
    FBone Tail;

    struct FDamage
    {
        float Heavy = 20;
        float Weak = 2;
    };
    FDamage DamageStrength;
    
    struct FRadius
    {

        float Damage;
        float LineTrace;
    }
    FRadius Radius;

    FName WeakDamageLimb;
    float Radius = 50;

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
public:
    UNotifyDamageTaken();
};
