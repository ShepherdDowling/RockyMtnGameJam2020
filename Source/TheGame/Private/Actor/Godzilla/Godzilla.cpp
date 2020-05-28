// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Actor/Godzilla/Godzilla.h"
#include "Asset/DefaultHUD.h"
#include "Mode/DefaultGameMode.h"
#include "Support/Rock.h"
#include "Support/Animate.h"
#include "Support/CollisionHandler.h"
#include "Support/DualHandle.h"

#include "UObject/UObjectGlobals.h" 
#include "UObject/ConstructorHelpers.h" 
#include "UObject/SparseDelegate.h" 

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Animation/AnimSequence.h" 
#include "Animation/AnimationAsset.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Engine/World.h" 
#include "Math/UnrealMathUtility.h" 
#include "TimerManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Delegates/DelegateSignatureImpl.inl" 



// Called to bind functionality to input
void AGodzilla::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("RightTailWhip"), IE_Pressed, this, &AGodzilla::RightTailWhip);
    PlayerInputComponent->BindAction(TEXT("Bite"), IE_Pressed, this, &AGodzilla::Bite);

    PlayerInputComponent->BindAction(TEXT("FaceLock"), IE_Pressed, this, &AGodzilla::FaceLock);
}


void AGodzilla::FaceLock()
{
    UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("face lock"));

    auto GameMode = Cast<ADefaultGameMode>(GetWorld()->GetAuthGameMode());
    if (!ensure(GameMode))
        return;

    ABaseCharacter* Target = nullptr;
    for (auto Pawn : GameMode->GetPlayerPawnArray())
    {
        if (Pawn != this)
        {
            Target = Pawn;
            break;
        }
    }
    if (!Target)
        return;

    auto Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetTargetLocation(), Target->GetTargetLocation());
    GetMovementComponent()->UpdatedComponent->SetWorldRotation(Rotation);
}

void AGodzilla::RightTailWhip()
{
    Animate->Animate(TEXT("Attack/RightTailWhip/RightTailWhipMT"), true);
}

void AGodzilla::Bite()
{
    Animate->Animate(TEXT("Attack/Bite/BiteMT"), true);
}


// Sets default values
AGodzilla::AGodzilla()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(true);
    ensure(Animate);
    UAnimate::SetHome(TEXT("/Game/_Actors/Godzilla/Animations"));
    Animate->SetActor(Cast<ABaseCharacter>(this));

    Animate->Add(UAnimate::NewAnimation(TEXT("Attack/RightTailWhip/RightTailWhipMT")));
    Animate->Add(UAnimate::NewAnimation(TEXT("Attack/Bite/BiteMT")));
    Animate->Add(UAnimate::NewAnimation(TEXT("Movement/Die")));

    //DualHandle = CreateDefaultSubobject<UDualHandle>("DualHandle");
}


AGodzilla::~AGodzilla()
{

}

// Called when the game starts or when spawned
void AGodzilla::BeginPlay()
{
    Super::BeginPlay();
    ensure(Animate);

    GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AGodzilla::OnCompBeginOverlap);
    GetMesh()->OnComponentEndOverlap.AddDynamic(this, &AGodzilla::OnCompEndOverlap);

    //DualHandle->Init(this, ARock::GetActorComponent(this, TEXT("HeadCapsule")), TEXT("Head_M"));
}

// Called every frame
void AGodzilla::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //DualHandle->UpdateGrip();
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
    {
        Animate->Animate("Movement/Die");
        OnDeath.Broadcast();
    }

    // TODO: GODZILLA FALLS BACKWARDS
    //Animate->Animate(TEXT("Ninja/Fall/Backwards"), true, true);
    return DamageToApply;
}

void AGodzilla::OnCompBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
    //UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("OnCompBeginOverlap"));
    if(this != otherActor)
        CollisionHandler->SetCollidingActor(otherActor);
}

void AGodzilla::OnCompEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
    //UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *FString("OnCompEndOverlap"));
    CollisionHandler->SetCollidingActor(nullptr);
}
