// Fill out your copyright notice in the Description page of Project Settings.


#include "Support/Rock.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h" 


UPrimitiveComponent* ARock::GetActorComponent(const AActor* Actor, const FString& Name)
{
    if (!Actor)
        return nullptr;
    for (UActorComponent* Components : Actor->GetComponents())
    {
        auto PrimComp = Cast<UPrimitiveComponent>(Components);
        if (!PrimComp)
            continue;
        // UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *PrimComp->GetName());
        if (PrimComp->GetName() == Name)
            return PrimComp;
    }
    return nullptr;
}



UPrimitiveComponent* ARock::GetActorComponent(const AActor* Actor, FString&& Name)
{
    return ARock::GetActorComponent(Actor, Name);
}

UPrimitiveComponent* ARock::GetActorComponentByRegex(const AActor* Actor, const FRegexPattern& Regex)
{
    if (!Actor)
        return nullptr;
    for (UActorComponent* Components : Actor->GetComponents())
    {
        auto PrimComp = Cast<UPrimitiveComponent>(Components);
        if (!PrimComp)
            continue;
        // UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *PrimComp->GetName());
        FRegexMatcher Matcher(Regex, PrimComp->GetName());
        if (Matcher.FindNext())
            return PrimComp;
    }
    return nullptr;
}

UPrimitiveComponent* ARock::GetActorComponentByRegex(const AActor* Actor, FRegexPattern&& Regex)
{
    return ARock::GetActorComponentByRegex(Actor, Regex);
}
