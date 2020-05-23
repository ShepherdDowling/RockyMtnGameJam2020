// Fill out your copyright notice in the Description page of Project Settings.


#include "Support/DualHandle.h"

#include "Components/PrimitiveComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h"


// Sets default values
ADualHandle::ADualHandle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADualHandle::Init(ACharacter* ThisCharacter, UPrimitiveComponent* TargetComponent, FName&& BoneName)
	: ThisChracter(ThisCharacter), TargetComponent(TargetComponent), BoneName(BoneName)
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

ADualHandle::~ADualHandle()
{
}

// Called when the game starts or when spawned
void ADualHandle::BeginPlay()
{
	Super::BeginPlay();
	
	auto GrabTarget[this](UPhysicsHandleComponent* Handle, float Offset) -> void
	{
		Handle->GrabComponentAtLocationWithRotation(
			Cast<UPrimitiveComponent>(TargetComponent),
			NAME_None,
			ThisCharacter->GetMesh()->GetBoneLocation(BoneName) + FVector(Offset, Offset, Offset),
			FRotator(0, 0, 0)
		);
	};
	GrabTarget(One,  GrabOffset);
	GrabTarget(Two, -GrabOffset);
}

// Called every frame
void ADualHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	One->SetTargetLocation(ThisCharacter->GetMesh()->GetBoneLocation(BoneName) + FVector( GrabOffset,  GrabOffset,  GrabOffset));
	Two->SetTargetLocation(ThisCharacter->GetMesh()->GetBoneLocation(BoneName) + FVector(-GrabOffset, -GrabOffset, -GrabOffset));
}

