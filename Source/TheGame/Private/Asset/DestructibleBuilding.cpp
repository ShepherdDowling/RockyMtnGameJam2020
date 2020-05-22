// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset/DestructibleBuilding.h"
#include "DestructibleMesh.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformProcess.h" 
#include "engine/EngineTypes.h"
#include "CoreMinimal.h"
#include "Engine/SkeletalMesh.h" 

#include "Support/Watch.h"

#include "Components/PrimitiveComponent.h" 
#include "Components/StaticMeshComponent.h" 
#include "DestructibleComponent.h"

#include "Components/SphereComponent.h" 
#include "Components/SceneComponent.h" 

#include "PhysicsEngine/PhysicsConstraintComponent.h" 
#include "PhysicsEngine/ConstraintInstance.h" 

#include "Kismet/GameplayStatics.h"

ADestructibleBuilding::ADestructibleBuilding()
{
    SetActorTickEnabled(false);
    PrimaryActorTick.bCanEverTick = true;

    DestructableComponent = CreateDefaultSubobject<UDestructibleComponent>("TheDestructableComponent");
    DestructableComponent->SetSimulatePhysics(true);
    DestructableComponent->SetNotifyRigidBodyCollision(true);
    DestructableComponent->SetCollisionProfileName("Destructible"); // view editor for specifics
    DestructableComponent->OnComponentHit.AddDynamic(this, &ADestructibleBuilding::OnCompHit);
    
    SetRootComponent(DestructableComponent);
    Watch = new FWatch;
}

ADestructibleBuilding::~ADestructibleBuilding()
{
    if (Watch)
        delete Watch;
}

void ADestructibleBuilding::BeginPlay()
{
    Super::BeginPlay();

    SetActorTickEnabled(false);
    BuidlingStartZ = GetTransform().GetLocation().Z;

    auto dm = Cast<USkeletalMesh>(DestructableComponent->GetDestructibleMesh());
    if (dm)
        BuildingHeight = dm->GetBounds().GetBox().GetSize().Z; // Z changes
    else
        BuildingHeight = 100000;

    LocationLastTick = GetTransform().GetLocation();
    StartLocation = LocationLastTick;
}

void ADestructibleBuilding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Use this if you want to prevent the building from sinking all the way through
    if (Watch->TimerIsRunning())
        return;

    GetTransform().GetLocation() += FVector(0, 0, -0.2);

    float WorldScaling = 0.2; 
    float BufferDistance = 50000;

    if(BuidlingStartZ > GetTransform().GetLocation().Z + (BuildingHeight * WorldScaling + BufferDistance))
        Destroy();
    

    if (PhysicsEnabled)
    {
        PhysicsEnabled = false;
        // TODO: Wthis does not ignore my building, why?
        DestructableComponent->SetSimulatePhysics(false);
        DestructableComponent->SetNotifyRigidBodyCollision(false);
        DestructableComponent->SetCollisionProfileName("NoCollision");
        DestructableComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        DestructableComponent->OnComponentHit.RemoveDynamic(this, &ADestructibleBuilding::OnCompHit);
    }
}

void ADestructibleBuilding::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    auto InstegatedController = OtherActor->GetInstigatorController();
    if (!InstegatedController) return; // occurse if you were hit by a bone (sub component)

    auto DamageCauser = InstegatedController->GetPawn();
    if (!ensure(DamageCauser)) return;

    auto IgnoredActors = TArray<AActor*>{ DamageCauser };
    const bool bDoFullDamage = true;

    UGameplayStatics::ApplyRadialDamage(this, DamageTaken, Hit.ImpactPoint, WallStrength, TSubclassOf<UDamageType>(), 
        IgnoredActors, DamageCauser, InstegatedController, bDoFullDamage, ECollisionChannel::ECC_Visibility);


    // Instead, reset the timer if you want to prevent it form sinking all the way through
    if(!TimerBuildingCollapseStarted)
        Watch->SetTimer(static_cast<double>(DebrisTimeout));
    //else
    //    SetActorTickEnabled(true); // TODO: Decide what to do with the disappearing bricks
    // I am using static mobility instead 2 lines down

    TimerBuildingCollapseStarted = true;
}

