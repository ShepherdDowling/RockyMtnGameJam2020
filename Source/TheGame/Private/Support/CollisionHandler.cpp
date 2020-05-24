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
#include "Components/ArrowComponent.h"

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

void UCollisionHandler::ModifyDirectional(const FVector& DirectionalRef, float X, float Y)
{
	// Note: Only X or Y will have a value.
	// That means that either X or Y will equal 0
	// If you are using a controller, you will fire both with the stick (but seperatly),
	// unless you are directly UP/Down/Left/Right

	UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("============================="));

	UE_LOG(LogTemp, Warning, TEXT("X,Y: %f	%f"), X, Y);
	UE_LOG(LogTemp, Warning, TEXT("LOG: DF = %s"), *DirectionalRef.ToString());

	if (Self.Actor == Other.Actor || (!Other.Actor) || (!Other.Mesh))
		return;

	UE_LOG(LogTemp, Warning, TEXT("LOG: %s: %s"), *FString("started"), *Other.Actor->GetName());

	Collision.Front = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Front), Empty.Quat, Empty.Shape);
	Collision.Back  = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Back),  Empty.Quat, Empty.Shape);
	Collision.Left  = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Left),  Empty.Quat, Empty.Shape);
	Collision.Right = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Right), Empty.Quat, Empty.Shape);
	Free = Collision.Reverse();

	UE_LOG(LogTemp, Warning, TEXT("LOG: %d	%d	%d	%d"), Collision.Front, Collision.Back, Collision.Left, Collision.Right);

	FMovement Move(X, Y, GetThisCharacter(), &DirectionalRef);

	if (Collision.AllClear() || Collision.AllBlocked())
	{
		Move.Character();
		return;
	}

	FDirection Facing(Self.Actor->GetTransform().GetRotation().GetAxisX()); // Gives X,Y,Z Rotation
	UE_LOG(LogTemp, Warning, TEXT("LOG: Direction = %s"), *Facing.Vec.ToString());

	if (FMath::RoundToInt(X) == FMath::RoundToInt(Facing.Vec.X) || FMath::RoundToInt(Y) == FMath::RoundToInt(Facing.Vec.Y))
	{
		if (X)
		{
			
			
		}
		else // Y
		{

		}
	}
	else 
	{
		if (Y)
			GetThisCharacter()->AddMovementInput(DirectionalRef, Y);
		else
			GetThisCharacter()->AddMovementInput(DirectionalRef, X);
	}

}
// --------------------------------------------------------------------------------------------------------------------------------------------
UCollisionHandler::FMovement::FMovement(float iX, float iY, ACharacter* iThisCharacter, const FVector* iDirectionalRefPtr)
	: X(iX), Y(iY), ThisCharacter(iThisCharacter), DirectionalRefPtr(iDirectionalRefPtr)
{
}

bool UCollisionHandler::FMovement::Up() const {
	return Y < 0;
}

bool UCollisionHandler::FMovement::Down() const {
	return Y > 0;
}

bool UCollisionHandler::FMovement::Left() const {
	return X < 0;
}

bool UCollisionHandler::FMovement::Right() const {
	return X > 0;
}

void UCollisionHandler::FMovement::Character()
{
	if (Y)
		ThisCharacter->AddMovementInput(*DirectionalRefPtr, Y);
	else
		ThisCharacter->AddMovementInput(*DirectionalRefPtr, X);
}
// --------------------------------------------------------------------------------------------------------------------------------------------
UCollisionHandler::FDirection::FDirection(FVector&& iVector)
	: Vec(iVector)
{
}

bool UCollisionHandler::FDirection::Up() const {
	return Vec.Y < 0;
}

bool UCollisionHandler::FDirection::Down() const {
	return Vec.Y > 0;
}

bool UCollisionHandler::FDirection::Left() const {
	return Vec.X < 0;
}

bool UCollisionHandler::FDirection::Right() const {
	return Vec.X > 0;
}


void UCollisionHandler::FCollision::operator=(const FCollision& Other)
{
	Front = Other.Front;
	Back = Other.Back;
	Left = Other.Left;
	Right = Other.Right;
}

bool UCollisionHandler::FCollision::AllClear() const
{
	return Front && Back && Left && Right;
}

bool UCollisionHandler::FCollision::AllBlocked() const
{
	return (!Front) && (!Back) && (!Left) && (!Right);
}

UCollisionHandler::FCollision UCollisionHandler::FCollision::Reverse() const
{
	FCollision Rev;
	Rev.Front = !Front;
	Rev.Back  = !Back;
	Rev.Left  = !Left;
	Rev.Right = !Right;
	return Rev;
}
// --------------------------------------------------------------------------------------------------------------------------------------------
UCollisionHandler::FMeshCompass::FMeshCompass()
	: Front(FName("Front")), Back(FName("Back")), Left(FName("Left")), Right(FName("Right")) 
{}

FVector UCollisionHandler::FMeshCompass::GetLocation(ESocket SocketName)
{
	switch (SocketName)
	{
	case ESocket::Front:
		return ThisCharacter->GetMesh()->GetSocketLocation(Front);
	case ESocket::Back:
		return ThisCharacter->GetMesh()->GetSocketLocation(Back);
	case ESocket::Left:
		return ThisCharacter->GetMesh()->GetSocketLocation(Left);
	case ESocket::Right:
		return ThisCharacter->GetMesh()->GetSocketLocation(Right);
	default:
		return FVector();
	};
}
// --------------------------------------------------------------------------------------------------------------------------------------------
