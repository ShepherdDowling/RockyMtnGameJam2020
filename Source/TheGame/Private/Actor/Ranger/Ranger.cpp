// Copyright Epic Games, Inc. All Rights Reserved.

#include "Actor/Ranger/Ranger.h"
#include "Actor/BaseCharacter.h"
#include "Support/Rock.h"
#include "Support/Animate.h"
#include "Widget/DefaultUI.h"

#include "UObject/UObjectGlobals.h" 
#include "UObject/ConstructorHelpers.h" 
#include "Engine/World.h" 
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h" 

#include "Animation/AnimSequence.h" 
#include "Animation/AnimationAsset.h" 
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ARanger::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &ARanger::Roll);
	PlayerInputComponent->BindAction(TEXT("Punch"), IE_Pressed, this, &ARanger::Punch);
	PlayerInputComponent->BindAction(TEXT("Kick"), IE_Pressed, this, &ARanger::Kick);
}

void ARanger::Roll()
{
	Animate->Animate(TEXT("Ninja/Roll/Backwards"), true);
}

void ARanger::Punch()
{
	Animate->Animate(TEXT("Ninja/Punch/Cross"), true);
}

void ARanger::Kick()
{
	if (ensure(Animate))
		Animate->Animate(TEXT("Ninja/Kick/HighMT"));
}

void ARanger::FallForwards()
{
}

void ARanger::FallBackwards()
{
}


ARanger::ARanger()
{
	UAnimate::SetHome(TEXT("/Game/_Actors/Ranger/Animations"));
	ensure(Animate);
	Animate->SetActor(Cast<ABaseCharacter>(this));
	Animate->Add(UAnimate::NewAnimation(TEXT("Ninja/Roll/Backwards")));
	Animate->Add(UAnimate::NewAnimation(TEXT("Ninja/Punch/Cross")));
	Animate->Add(UAnimate::NewAnimation(TEXT("Ninja/Kick/HighMT")));

	Animate->Add(UAnimate::NewAnimation(TEXT("Ninja/Fall/Forwards")));
	Animate->Add(UAnimate::NewAnimation(TEXT("Ninja/Fall/Backwards")));

	bUseControllerRotationYaw = false;
}

ARanger::~ARanger()
{
	//UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("Ranger Destructor Called"));
}

void ARanger::BeginPlay()
{
	Super::BeginPlay();
	ensure(Animate);
}

void ARanger::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
}

float ARanger::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	bool bDoDamage = false;
	if (!ensure(EventInstigator)) return 0;

	ABaseCharacter* GP = Cast<ABaseCharacter>(EventInstigator->GetPawn());

	if (!GP)
		bDoDamage = true;
	else if(GP->GetFName() != GetFName())  // we don't apply damage to ourself if radial damage is used
	{
		if (!OpponentDamageTrackMap.Contains(GP->GetFName()))
		{
			bDoDamage = true;
			OpponentDamageTrackMap.Add(GP->GetFName(), GP->GetTotalAnimationsPlayed());
		}
		else
		{
			if (OpponentDamageTrackMap[GP->GetFName()] != GP->GetTotalAnimationsPlayed())
			{
				OpponentDamageTrackMap[GP->GetFName()] = GP->GetTotalAnimationsPlayed();
				bDoDamage = true;
			}
		}
	}

	if (!bDoDamage)
		return 0;

	int32 RoundedDamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<int32>(RoundedDamagePoints, 0, Health.Current);
	Health.Current -= DamageToApply;

	if (Health.Current <= 0)
		OnDeath.Broadcast();

	Animate->Animate(TEXT("Ninja/Fall/Backwards"), true, true);
	return DamageToApply;
}
