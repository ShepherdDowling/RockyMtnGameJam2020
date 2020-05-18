// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Godzilla/Godzilla.h"
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


// Called to bind functionality to input
void AGodzilla::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("TailWhip"), IE_Pressed, this, &AGodzilla::TailWhip);
}


void AGodzilla::TailWhip()
{
	UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("Whip"));
	Animate->Animate(TEXT("Attack/TailWhip/TailWhipMT"), true);
}


// Sets default values
AGodzilla::AGodzilla()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ensure(Animate);
	UAnimate::SetHome(TEXT("/Game/_Actors/Godzilla/Animations"));
	Animate->SetActor(Cast<ABaseCharacter>(this));

	Animate->Add(UAnimate::NewAnimation(TEXT("Attack/TailWhip/TailWhipMT")));
}


AGodzilla::~AGodzilla()
{

}

// Called when the game starts or when spawned
void AGodzilla::BeginPlay()
{
	Super::BeginPlay();
	ensure(Animate);
}

// Called every frame
void AGodzilla::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AGodzilla::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	bool bDoDamage = false;
	if (!ensure(EventInstigator)) return 0;

	ABaseCharacter* GP = Cast<ABaseCharacter>(EventInstigator->GetPawn());

	if (!GP)
		bDoDamage = true;
	else if (GP->GetFName() != GetFName())  // we don't apply damage to ourself if radial damage is used
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

	// TODO: GODZILLA FALLS BACKWARDS
	//Animate->Animate(TEXT("Ninja/Fall/Backwards"), true, true);
	return DamageToApply;
}


