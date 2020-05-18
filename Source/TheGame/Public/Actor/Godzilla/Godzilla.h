// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseCharacter.h"
#include "Godzilla.generated.h"


class UDefaultUI;
class UAnimSequence;
class UAnimate;

#define AnimatorBP ConstructorHelpers::FObjectFinder<UAnimSequence>


UCLASS(config = Game)
class AGodzilla : public ABaseCharacter
{
	GENERATED_BODY()

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:

	AGodzilla();
	virtual ~AGodzilla();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};

