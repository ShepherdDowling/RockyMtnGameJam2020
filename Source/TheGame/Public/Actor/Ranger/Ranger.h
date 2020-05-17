// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseCharacter.h"
#include "Ranger.generated.h"


class UDefaultUI;
class UAnimSequence;
class UAnimate;

#define AnimatorBP ConstructorHelpers::FObjectFinder<UAnimSequence>


UCLASS(config=Game)
class ARanger : public ABaseCharacter
{
	GENERATED_BODY()

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Roll();
	void Punch();
	void Kick();

	void FallForwards();
	void FallBackwards();

public:

	ARanger();
	virtual ~ARanger();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};

