// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Godzilla/NotifyDamageTaken.h"

#include "Actor/BaseCharacter.h"
#include "Support/Rock.h"


#include "UObject/UObjectGlobals.h" 
#include "UObject/ConstructorHelpers.h" 
#include "UObject/SparseDelegate.h" 

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimSequence.h" 
#include "Animation/AnimationAsset.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Engine/World.h" 
#include "Math/UnrealMathUtility.h" 
#include "TimerManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Delegates/DelegateSignatureImpl.inl" 

// TODO: REMOVE UN-NEEDED HEADERS ABOVE
// ---------------------------------------------------

void UNotifyDamageTaken::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("Hit from C++"));

    auto Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
    if (!Character)
        return;

    
    FHitResult HitData;

    // Blueprint known as SphereTraceByChannel
    UKismetSystemLibrary::SphereTraceSingle(
        MeshComp,
        MeshComp->GetBoneLocation(Tail.Start),
        MeshComp->GetBoneLocation(Tail.End),
        Radius,
        ETraceTypeQuery::TraceTypeQuery1,
        false,          // Trace Complex
        TArray<AActor*>(), // Characters to ignore (bool for us later)
        EDrawDebugTrace::Type::ForDuration,
        HitData,        // Out param for hit data
        true,           // Dont' hit yourself
        ARock::Red,     // Trace Color
        ARock::Green,   // Hit Color
        5.0f            // Draw Time        
    );

    if (!HitData.Actor.Get())
        return;

    float Damage = 0;
    if (HitData.BoneName == WeakDamageLimb)
        Damage = DamageStrength.Weak;
    else
        Damage = DamageStrength.Heavy;

    UGameplayStatics::ApplyRadialDamage(
        HitData.Actor.Get(),
        Damage,
        HitData.ImpactPoint,
        Radius,

    );
}

UNotifyDamageTaken::UNotifyDamageTaken()
{
    Tail.Start = TEXT("Tail1_M");
    Tail.End = TEXT("Tail6_M");
    WeakDamageLimb = TEXT("Tail0_M");
}
