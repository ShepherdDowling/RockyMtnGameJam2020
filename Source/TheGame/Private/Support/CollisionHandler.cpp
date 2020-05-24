// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Support/CollisionHandler.h"
#include "Support/Rock.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UObject/UObjectGlobals.h" 
#include "Engine/World.h" 

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"

#include "Engine/World.h" 

// Sets default values
UCollisionHandler::UCollisionHandler()
{
	MeshStrPattern = new FRegexPattern(TEXT(R"(^.*([Mm]esh).*$)"));
}

UCollisionHandler::~UCollisionHandler()
{
	if (MeshStrPattern)
		delete MeshStrPattern;
}

void UCollisionHandler::Init(ACharacter* thisCharacter)
{
	Self.Actor = thisCharacter;
	Self.Mesh = ARock::GetActorComponentByRegex(Self.Actor, *MeshStrPattern); // Default for USkeletalMeshComponent (Character Mesh)

	ensure(Self.Actor);
	if (!ensure(Self.Mesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("Has no mesh: %s"), *Self.Actor->GetName());
	}
	MeshCompass.ThisCharacter = GetThisCharacter();
}

USkeletalMeshComponent* UCollisionHandler::GetMesh() const {
	return Cast<USkeletalMeshComponent>(Self.Mesh);
}

ACharacter* UCollisionHandler::GetThisCharacter() const {
	return Cast<ACharacter>(Self.Actor);
}

void UCollisionHandler::SetCollidingActor(AActor* actor) 
{
	if (Other.Actor == actor)
		return;
	else if (!actor)
		return;

	Other.Actor = actor;
	Other.Mesh = ARock::GetActorComponentByRegex(Other.Actor, *MeshStrPattern);  // Default for USkeletalMeshComponent (Character Mesh)
	if (!Other.Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has no mesh: %s"), *Other.Actor->GetName());
		Other.Actor = nullptr;
	}
}

AActor* UCollisionHandler::GetCollidingActor() const {
	return Other.Actor;
}

FVector UCollisionHandler::GetCollisionLocation()
{
	return Other.Actor->GetTargetLocation();
}

bool UCollisionHandler::AllClear() const
{
	
	return false;
}

bool UCollisionHandler::AllBlocked() const
{
	return false;
}

void UCollisionHandler::ModifyDirectional(const FVector& DirectionalRef, float X, float Y)
{
	if (!(Other.Actor || Other.Mesh))
		return;

	Collision.Front = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Front), Empty.Quat, Empty.Shape);
	Collision.Back  = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Back), Empty.Quat, Empty.Shape);
	Collision.Left  = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Left), Empty.Quat, Empty.Shape);
	Collision.Right = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Right), Empty.Quat, Empty.Shape);

	if (AllClear())
	{
		Other.Actor = nullptr;
	}
	else if (AllBlocked())
	{
		if(Y)
			GetThisCharacter()->AddMovementInput(DirectionalRef, Y);
		else
			GetThisCharacter()->AddMovementInput(DirectionalRef, X);
	}

	if (Collision.Left && X < 0)
		GetThisCharacter()->AddMovementInput(DirectionalRef, Y);
	else if (Collision.Right && X > 0)
		GetThisCharacter()->AddMovementInput(DirectionalRef, Y);
	else if (Collision.Front && Y < 0)
		GetThisCharacter()->AddMovementInput(DirectionalRef, Y); // Make movement
	else if (Collision.Back && Y > 0) 
		GetThisCharacter()->AddMovementInput(DirectionalRef, Y); // Make movement
	else{
		Other.Actor = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("No Collision"));
	}
}

bool UCollisionHandler::FCollision::AllClear() const
{
	return Front && Back && Left && Right;
}

bool UCollisionHandler::FCollision::AllBlocked() const
{
	return (!Front) && (!Back) && (!Left) && (!Right);
}

UCollisionHandler::FMeshCompass::FMeshCompass()
	: Front(FName("Front")), Back(FName("Back")), Left(FName("Left")), Right(FName("Right")) 
{}

FVector UCollisionHandler::FMeshCompass::GetLocation(ESocket SocketName)
{
	switch (SocketName)
	{
	case ESocket::Front:
		return ThisCharacter->GetMesh()->GetBoneLocation(Front);
	case ESocket::Back:
		return ThisCharacter->GetMesh()->GetBoneLocation(Back);
	case ESocket::Left:
		return ThisCharacter->GetMesh()->GetBoneLocation(Left);
	case ESocket::Right:
		return ThisCharacter->GetMesh()->GetBoneLocation(Right);
	default:
		return FVector();
	};
}


