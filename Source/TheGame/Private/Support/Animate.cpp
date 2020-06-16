// Copyright by Shepherd Dowling under the Apache v2 licence

#include "Support/Animate.h"
#include "Support/Watch.h"
#include "Actor/BaseCharacter.h"

#include "GameFramework/Character.h"
#include "UObject/UObjectGlobals.h"

#include "Kismet/GameplayStatics.h"
#include "Internationalization/Regex.h"
#include "Containers/Map.h"
#include "TimerManager.h"


FString UAnimate::Home;

void UAnimate::SetBlueprintHandleInvalid()
{

    if (ReturnToBlueprintHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(ReturnToBlueprintHandle);
        ReturnToBlueprintHandle.Invalidate();

    }
}

UAnimate::UAnimate()
{
    AnimationInPlay = new FWatch;
}


UAnimate::~UAnimate()
{
    if (AnimationInPlay)
        delete AnimationInPlay;
}

void UAnimate::SetHome(const FString& NewHome)
{
    UAnimate::Home = NewHome.Replace(TEXT("//"), TEXT("/"));
}

FString UAnimate::GetHome()
{
    return UAnimate::Home;
}

const TPair<FName, UAnimSequenceBase*> UAnimate::NewAnimation(const FString& ExtendedPath)
{
    FString FullPath = FString(UAnimate::Home + '/' + ExtendedPath).Replace(TEXT("//"), TEXT("/"));
    TPair<FName, UAnimSequenceBase*> pair(ExtendedPath, LoadObject<UAnimSequenceBase>(NULL, *FullPath, nullptr, 0));
    if (pair.Value)
        pair.Value->AddToRoot();
    return pair;
}

void UAnimate::SetActor(ABaseCharacter* iActor)
{
    if(iActor != nullptr)
        Actor = iActor;
}

void UAnimate::Add(const TPair<FName, UAnimSequenceBase*>& MapPair)
{
    Map.Add(MapPair);

    if (!ensure(Map[MapPair.Key]))
    {
        UE_LOG(LogTemp, Error, TEXT("Animation Failed Init: %s"), *MapPair.Key.ToString());
        Map.Remove(MapPair.Key);
    }
}

void UAnimate::AddFileKey(const FName& AnimationPath)
{
    FRegexMatcher Matcher(*Regex, AnimationPath.ToString());
    if (!Matcher.FindNext())
        return;

    FString FullPath = UAnimate::Home + Matcher.GetCaptureGroup(1) + Matcher.GetCaptureGroup(2);
    FString CleanFullPath = FullPath.Replace(TEXT("(//)"), TEXT("/"));

    FName Key(Matcher.GetCaptureGroup(2));
    Map.Add(Key, LoadObject<UAnimSequenceBase>(NULL, *FullPath, nullptr, 0));
    if (!ensure(Map[Key]))
    {
        UE_LOG(LogTemp, Error, TEXT("Animation Failed Init: %s"), *Matcher.GetCaptureGroup(2));
    }
}


void UAnimate::Animate(const FName& str, bool bResetToBlueprint, bool bOverride)
{
    if (!ensure(Map.Contains(str)))
    {
        UE_LOG(LogTemp, Error, TEXT("Animation Key Not Found: %s"), *str.ToString());
        return;
    }
    else if (!ensure(Map[str]))
    {
        UE_LOG(LogTemp, Error, TEXT("Animation Value Not Found: %s"), *str.ToString());
        return;
    }

    if (AnimationInPlay->GetTimeRemaining() > 0 && !bOverride)
        return;

    SetBlueprintHandleInvalid();
    bRunningBlueprint = false;

    Actor->GetMesh()->PlayAnimation(Cast<UAnimSequenceBase>(Map[str]), false);
    //Actor->PlayAnimMontage(Map[str], 1.0);

    TotalAnimationsPlayed++;
    AnimationInPlay->SetTimer(static_cast<double>(Map[str]->SequenceLength));

    if (bResetToBlueprint)
        ResetBlueprintInSeconds();
}

void UAnimate::ResetBlueprint()
{
    bRunningBlueprint = true;
    Actor->GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
}

void UAnimate::ResetBlueprintInSeconds(float Seconds)
{
    if (!Seconds)
        Seconds = AnimationInPlay->GetTimeRemaining()-0.1;
        // Ensure that Montage will loop, so it won't give a snapping state to no animation
        // then pick up with the blueprint 1/10 of a second before the repeate starts to ensure
        // a smooth transition

    SetBlueprintHandleInvalid();
    GetWorld()->GetTimerManager().SetTimer(
        ReturnToBlueprintHandle,
        this,
        &UAnimate::ResetBlueprint,
        Seconds,
        false
    );
}

bool UAnimate::RunningBlueprint() const
{
    return bRunningBlueprint;
}
