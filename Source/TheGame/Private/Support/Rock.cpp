// Fill out your copyright notice in the Description page of Project Settings.


#include "Support/Rock.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h" 


UPrimitiveComponent* ARock::GetActorComponent(const AActor* Actor, const FString&& Name)
{
    for (UActorComponent* Components : Actor->GetComponents())
    {
        auto PrimComp = Cast<UPrimitiveComponent>(Components);
        if (!PrimComp)
            continue;
        if (PrimComp->GetName() == Name)
            return PrimComp;
        
    }
    return nullptr;
}


