// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Godzilla/GodzillaAMI.h"
#include "Actor/Godzilla/Godzilla.h"

#include "GameFramework/Character.h" 
#include "GameFramework/CharacterMovementComponent.h" 

UGodzillaAMI::UGodzillaAMI()
{
}

UGodzillaAMI::~UGodzillaAMI()
{
}

void UGodzillaAMI::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    AGodzilla* OwningActor = Cast<AGodzilla>(GetOwningActor());
    if (!OwningActor)
        return;


    Speed = OwningActor->GetVelocity().Size();
    bIsInAir = OwningActor->GetCharacterMovement()->IsFalling();
}
