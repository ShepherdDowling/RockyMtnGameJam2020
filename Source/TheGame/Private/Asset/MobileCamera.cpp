
// Copyright by Shepherd Dowling under the Apache v2 licence

#include "Asset/MobileCamera.h"
#include "Support/Rock.h"
#include "Mode/DefaultGameMode.h"
#include "Actor/BaseCharacter.h"


#include "GameFramework/PlayerController.h" 
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h" 
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h" 
#include "kismet/GameplayStatics.h" // For View Target with Blend


void AMobileCamera::UpdateSpringArmLength()
{
	// TODO: Update this so it works with more than 2 players
	if ((*BaseCharacterListPtr).Num() < 2)
		return;

	auto vActorDistance = FMath::Clamp<float>(
		(*BaseCharacterListPtr)[0]->GetDistanceTo((*BaseCharacterListPtr)[1]),
		MinArmDistance,
		MaxArmDistance
		);

	SpringArm->TargetArmLength = vActorDistance;
}

void AMobileCamera::UpdateCameraLocation()
{
	// TODO: Update this so it works with more than 2 players
	TargetLocation = FVector(0, 0, 0);

	float ClosestActorX = GetFirstPlayerLocation.X;
	float FurthestActorX = GetFirstPlayerLocation.X;
	for (ABaseCharacter* BaseCharacter : *BaseCharacterListPtr)
	{
		if (!BaseCharacter)
			continue;
		auto Location = BaseCharacter->GetActorLocation();
		TargetLocation += Location;
		if (ClosestActorX > Location.X)
			ClosestActorX = Location.X;

		if (FurthestActorX < Location.X)
			FurthestActorX = Location.X;
	}
	TargetLocation /= GameModePtr->GetMaxPlayerCount();
	TargetLocation.X -= ((FurthestActorX - ClosestActorX) * 0.2);

	// SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), .02));
	SetActorLocation(TargetLocation);
}

// Called when the game starts or when spawned
void AMobileCamera::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = GetActorLocation();

	if (GetWorld()->GetCurrentLevel()->GetName() != "BattleGrounds")
		return;
}


// Sets default values
AMobileCamera::AMobileCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultScene = CreateDefaultSubobject<USceneComponent>(FName("DefaultScene"));
	SetRootComponent(DefaultScene);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->AttachToComponent(DefaultScene, FAttachmentTransformRules::KeepRelativeTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);

	SpringArm->bDoCollisionTest = false;
}

void AMobileCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowAndSpanPlayers();
}

void AMobileCamera::AddReferences(ADefaultGameMode* iGameModePtr, TArray<AActor*>* iPlayerStartsPtr, TArray<ABaseCharacter*>* iBaseCharacterListPtr)
{
	GameModePtr = iGameModePtr;
	PlayerStartsPtr = iPlayerStartsPtr;
	BaseCharacterListPtr = iBaseCharacterListPtr;
}


void AMobileCamera::LinkCameraAndActors()
{
	if (!GameModePtr)
		return;

	for (ABaseCharacter* BaseCharacter : *BaseCharacterListPtr)
	{
		if (!BaseCharacter)
			continue;
		Cast<APlayerController>(BaseCharacter->GetController())->SetViewTargetWithBlend(this);
	}
}

void AMobileCamera::FollowAndSpanPlayers()
{
	UpdateSpringArmLength();
	UpdateCameraLocation();
}
