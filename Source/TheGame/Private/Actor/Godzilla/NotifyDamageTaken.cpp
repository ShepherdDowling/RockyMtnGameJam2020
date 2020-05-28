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

void UNotifyDamageTaken::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    Super::NotifyTick(MeshComp, Animation, TotalDuration);

    auto Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
    if (!Character)
        return;

    
    FHitResult HitData;

    // Blueprint known as SphereTraceByChannel
    UKismetSystemLibrary::SphereTraceSingle(
        MeshComp,
        MeshComp->GetBoneLocation(Tail.End), // This is important (End of the spear goes first)!! 
        MeshComp->GetBoneLocation(Tail.Start), // (otherwise you may not get the bone name)
        Radius.LineTrace,
        ETraceTypeQuery::TraceTypeQuery1,
        false,          // Trace Complex
        TArray<AActor*>{ Character }, // Characters to ignore (bool for us later)
        EDrawDebugTrace::Type::None,
        HitData,        // Out param for hit data
        true,           // Dont' hit yourself
        ARock::Red,     // Trace Color
        ARock::Green,   // Hit Color
        5.0f            // Draw Time        
    );

    if (HitData.Actor.Get()) {
       // UE_LOG(LogTemp, Warning, TEXT(">>> Actor >>>: %s"), *HitData.Actor->GetName()); 
    }
    if (HitData.BoneName != ARock::NameNone) {
        UE_LOG(LogTemp, Warning, TEXT(">>> Limb  >>>: %s"), *HitData.BoneName.ToString());
    }

    if (!HitData.Actor.Get() || HitData.BoneName == ARock::NameNone)
        return;

    if (HitData.BoneName.ToString().Mid(0, 4) == WeakDamageLimb) // Mid is Non-Inclusive
        BaseDamage = DamageStrength.Weak;
    else
        BaseDamage = DamageStrength.Heavy;

    UGameplayStatics::ApplyRadialDamage(
        HitData.Actor.Get(),
        BaseDamage,
        HitData.ImpactPoint,
        Radius.Damage,
        TSubclassOf<UDamageType>(),
        TArray<AActor*>{Character},
        Character,
        Character->GetController(),
        true, // Do Full Damage,
        ECollisionChannel::ECC_Visibility
    );


}

UNotifyDamageTaken::UNotifyDamageTaken()
{
    Tail.Start = TEXT("Tail1_M");
    Tail.End = TEXT("Tail6_M");
    WeakDamageLimb = TEXT("Tail");
}
