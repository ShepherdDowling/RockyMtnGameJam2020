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


#include "Components/StaticMeshComponent.h" 
#include "DestructibleComponent.h"

#include "Components/PrimitiveComponent.h" 
#include "Components/SphereComponent.h" 
#include "Components/SceneComponent.h" 

#include "PhysicsEngine/PhysicsConstraintComponent.h" 
#include "PhysicsEngine/ConstraintInstance.h" 

#include "Kismet/GameplayStatics.h"

ADestructibleBuilding::ADestructibleBuilding()
{
    PrimaryActorTick.bCanEverTick = true;

    DestructableComponent = CreateDefaultSubobject<UDestructibleComponent>("TheDestructableComponent");
    DestructableComponent->SetSimulatePhysics(true);
    DestructableComponent->SetNotifyRigidBodyCollision(true);
    DestructableComponent->SetCollisionProfileName("Custom"); // view editor for specifics
    DestructableComponent->OnComponentHit.AddDynamic(this, &ADestructibleBuilding::OnCompHit);
    
    SetRootComponent(DestructableComponent);
    SetTickGroup(ETickingGroup::TG_PostPhysics);
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
    BuildingHeight = Cast<USkeletalMesh>(DestructableComponent->GetDestructibleMesh())->GetBounds().GetBox().GetSize().Z; // Z changes
    //DestructableComponent->GetDestructibleMesh()->set
    LocationLastTick = GetTransform().GetLocation();
    StartLocation = LocationLastTick;
}

void ADestructibleBuilding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (Watch->TimerIsRunning())
        return;

    SetActorLocation(GetTransform().GetLocation() + FVector(0, 0, -1));

    float WorldScaling = 0.2; 
    if(BuidlingStartZ > GetTransform().GetLocation().Z + (BuildingHeight*WorldScaling))
        Destroy();
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

    UE_LOG(LogTemp, Warning, TEXT("GetTimeRemaining: %f"), Watch->GetTimeRemaining());

    SetActorTickEnabled(true);

    UE_LOG(LogTemp, Warning, TEXT("LOG: %f"), DebrisTimeout);
    Watch->SetTimer(static_cast<double>(DebrisTimeout));
    TimerBuildingCollapseStarted = true;
}

