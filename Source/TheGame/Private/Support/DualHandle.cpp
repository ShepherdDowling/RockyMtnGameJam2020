// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Support/DualHandle.h"

#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h" 
#include "Components/StaticMeshComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h"


UDualHandle::UDualHandle()
{
	One = CreateDefaultSubobject<UPhysicsHandleComponent>("Handle One");
	Two = CreateDefaultSubobject<UPhysicsHandleComponent>("Handle Two");

	ensure(One);
	ensure(Two);

	One->SetLinearStiffness(Friction);
	Two->SetLinearStiffness(Friction);

	One->SetAngularStiffness(Friction);
	Two->SetAngularStiffness(Friction);
}


void UDualHandle::Init(ACharacter* iThisCharacter, UPrimitiveComponent* iTargetComponent, FName&& iBoneName)
{
	ThisCharacter	= iThisCharacter;
	TargetComponent = iTargetComponent;
	BoneName		= iBoneName;

	auto GrabTarget = [this](UPhysicsHandleComponent* Handle, float Offset) -> void
	{
		Handle->GrabComponentAtLocationWithRotation(
			Cast<UPrimitiveComponent>(TargetComponent),
			NAME_None,
			ThisCharacter->GetMesh()->GetBoneLocation(BoneName) + FVector(Offset, Offset, Offset),
			FRotator(0, 0, 0)
		);
	};
	GrabTarget(One, GrabOffset);
	GrabTarget(Two, -GrabOffset);
}

// Called every frame
void UDualHandle::UpdateGrip()
{
	One->SetTargetLocation(ThisCharacter->GetMesh()->GetBoneLocation(BoneName) + FVector( GrabOffset,  GrabOffset,  GrabOffset));
	Two->SetTargetLocation(ThisCharacter->GetMesh()->GetBoneLocation(BoneName) + FVector(-GrabOffset, -GrabOffset, -GrabOffset));
}

