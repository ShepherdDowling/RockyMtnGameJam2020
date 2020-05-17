// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Ranger/RangerAMI.h"
#include "Actor/Ranger/Ranger.h"

#include "GameFramework/Character.h" 
#include "GameFramework/CharacterMovementComponent.h" 

URangerAMI::URangerAMI()
{
}

URangerAMI::~URangerAMI()
{
}

void URangerAMI::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ARanger* OwningActor = Cast<ARanger>(GetOwningActor());
    if (!OwningActor)
        return;


    Speed = OwningActor->GetVelocity().Size();
    bIsInAir = OwningActor->GetCharacterMovement()->IsFalling();
}
