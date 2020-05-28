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
        float LineTrace = 50;
        float Damage = 20;
    };
    FRadius Radius;

    float BaseDamage = 0;
    FString WeakDamageLimb;

    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
public:
    UNotifyDamageTaken();
};
