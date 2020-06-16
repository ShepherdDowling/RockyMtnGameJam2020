// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Support/CollisionHandler.h"
#include "Support/Rock.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"


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

FVector UCollisionHandler::GetCollisionLocation(){
	return Other.Actor->GetTargetLocation();
}

void UCollisionHandler::ModifyDirectional(const FVector& DirectionalRef, float X, float Y)
{
	// Note: Only X or Y will have a value.
	// That means that either X or Y will equal 0
	// If you are using a controller, you will fire both with the stick (but seperatly),
	// unless you are directly UP/Down/Left/Right

	//UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("============================="));
	//UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("Started ModifyDirectional()"));

	if (Self.Actor == Other.Actor || (!Other.Actor) || (!Other.Mesh))
	{
		Self.Actor = nullptr;
		return;
	}

	Collision.Front = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Front), Empty.Quat, Empty.Shape);
	Collision.Back  = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Back),  Empty.Quat, Empty.Shape);
	Collision.Left  = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Left),  Empty.Quat, Empty.Shape);
	Collision.Right = Other.Mesh->OverlapComponent(MeshCompass.GetLocation(ESocket::Right), Empty.Quat, Empty.Shape);
	Free = Collision.Reverse();

	FMovement Move(GetThisCharacter(), &DirectionalRef, X, Y);

	if (Collision.AllClear() || Collision.AllBlocked())
	{
		Other.Actor = nullptr;
		return Move.Character();
	}

	FDirection Facing(Self.Actor->GetTransform().GetRotation().GetAxisX());
	//UE_LOG(LogTemp, Warning, TEXT("---------- U  D  L  R"));
	//UE_LOG(LogTemp, Warning, TEXT("Collision: %d  %d  %d  %d"), Collision.Front, Collision.Back, Collision.Left, Collision.Right);
	//UE_LOG(LogTemp, Warning, TEXT("Facing:    %d  %d  %d  %d"), Facing.Up(), Facing.Down(), Facing.Left(), Facing.Right());
	//UE_LOG(LogTemp, Warning, TEXT("Moving:    %d  %d  %d  %d"), Move.Up(), Move.Down(), Move.Left(), Move.Right());
	//UE_LOG(LogTemp, Warning, TEXT("Rounded:   %d  %d  %d  %d"), FMath::RoundToInt(X), FMath::RoundToInt(Facing.Vec.X), FMath::RoundToInt(Y), FMath::RoundToInt(Facing.Vec.Y));

	bool FacingMatched = (FMath::RoundToInt(X) == FMath::RoundToInt(Facing.Vec.X) && FMath::RoundToInt(Y) == FMath::RoundToInt(Facing.Vec.Y));

	if (Facing.Up())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %d"), 1);
		if (
			   (Free.Front && Move.Up())
			|| (Free.Back  && Move.Down())
			|| (Free.Left  && Move.Left())
			|| (Free.Right && Move.Right())
			|| (Move.Up() && Collision.Right && Facing.RightInPart())
			|| (Move.Up() && Collision.Left && Facing.LeftInPart())
			|| (Move.Up() && (Collision.Left || Collision.Right) && FacingMatched)
			)
		{
			return Move.Character();
		}
	}
	else if (Facing.Down())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %d"), 2);
		if
			(
				   (Collision.Back  && Move.Down())
				|| (Collision.Front && Move.Up())
				|| (Collision.Left  && Move.Left())
				|| (Collision.Right && Move.Right())
				)
		{
			return Move.Character();
		}
		else if
			(
				   (Free.Left  && Move.Right())
				|| (Free.Right && Move.Left())
				|| (Free.Back  && Move.Up())
				|| (Free.Front && Move.Down())
				|| (Move.Down() && Collision.Right && Facing.LeftInPart()) // Mirror
				|| (Move.Down() && Collision.Left && Facing.RightInPart())
				|| (Move.Down() && (Collision.Left || Collision.Right) && FacingMatched)
				)
		{
			return Move.Character();
		}
	}
	else if (Facing.Left())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %d"), 3);
		if (
			   (Move.Up()    && Free.Right)
			|| (Move.Down()  && Free.Left)
			|| (Move.Left()  && Free.Front)
			|| (Move.Right() && Free.Back)
			|| (Move.Left()  && (Collision.Right || Collision.Left))
			)
		{
			return Move.Character();
		}
	}
	else if (Facing.Right())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %d"), 4);
		if (
			   (Move.Up()    && Free.Left)
			|| (Move.Down()  && Free.Right)
			|| (Move.Left()  && Free.Back)
			|| (Move.Right() && Free.Front)
			|| (Move.Right() && (Collision.Right || Collision.Left))
			)
		{
			return Move.Character();
		}
	}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Not Facing Any Distinct Direction"));
	//	UE_LOG(LogTemp, Warning, TEXT("FACING:    %f  %f"), Facing.Vec.X, Facing.Vec.Y);
	//}
}
// --------------------------------------------------------------------------------------------------------------------------------------------
UCollisionHandler::FMovement::FMovement(ACharacter* iThisCharacter, const FVector* iDirectionalRefPtr, float iX, float iY)
	: ThisCharacter(iThisCharacter), DirectionalRefPtr(iDirectionalRefPtr), X(iX), Y(iY)
{
}

bool UCollisionHandler::FMovement::Up() const {
	return Y < -0.00001;
}

bool UCollisionHandler::FMovement::Down() const {
	return Y > 0.00001;
}

bool UCollisionHandler::FMovement::Left() const {
	return X < -0.00001;
}

bool UCollisionHandler::FMovement::Right() const {
	return X > 0.00001;
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
	return Vec.Y <= -0.7;
}

bool UCollisionHandler::FDirection::Down() const {
	return Vec.Y >= 0.7;
}

bool UCollisionHandler::FDirection::Left() const {
	return Vec.X <= -0.7;
}

bool UCollisionHandler::FDirection::Right() const {
	return Vec.X >= 0.7;
}

bool UCollisionHandler::FDirection::UpInPart() const {
	return Vec.Y <= -0.0001;
}

bool UCollisionHandler::FDirection::DownInPart() const {
	return Vec.Y >= 0.0001;
}

bool UCollisionHandler::FDirection::LeftInPart() const {
	return Vec.X <= -0.0001;
}

bool UCollisionHandler::FDirection::RightInPart() const {
	return Vec.X >= 0.0001;
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

