// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animate.generated.h"

class AActor;
class ABaseCharacter;
class FTimerManager;
class FWatch;

class UAnimMontage;
class UAnimSequence;
class UAnimSequenceBase;


// AnimSequence : AnimSequenceBase
// AnimMontage  : AnimCompositeBase: AnimSequenceBase


/**
 *  This class creates a hash-map of animations for an actor
 *  The class takes a base path and you use the key (filename) to access your animation
 */
UCLASS()
class THEGAME_API UAnimate : public UActorComponent
{
    GENERATED_BODY()
private:
    static FString Home;

    ACharacter* Actor = nullptr;

    TMap<FName, UAnimSequenceBase*> Map;
    FWatch* AnimationInPlay;
    int32 TotalAnimationsPlayed = 0;

    FRegexPattern* Regex = nullptr;

    FTimerHandle ReturnToBlueprintHandle;
    bool bRunningBlueprint = true;

    void SetBlueprintHandleInvalid();

public:
    UAnimate();
    virtual ~UAnimate();
    
    static void SetHome(const FString& NewHome);
    static FString GetHome();

    static const TPair<FName, UAnimSequenceBase*> NewAnimation(const FString& ExtendedPath);
    void SetActor(ABaseCharacter* iActor);

    void Add(const TPair<FName, UAnimSequenceBase*>& MapPair);
    void AddFileKey(const FName& AnimationPath);

    void Animate(const FName& str, bool bResetToBlueprint = true, bool bOverride = false);
    void ResetBlueprint();
    void ResetBlueprintInSeconds(float Seconds = 0);

    bool RunningBlueprint() const;
    FORCEINLINE int32 GetTotalAnimationsPlayed() const { return TotalAnimationsPlayed; }
};
