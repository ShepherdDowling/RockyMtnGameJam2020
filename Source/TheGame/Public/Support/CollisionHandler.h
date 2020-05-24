// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionHandler.generated.h"

class AActor;
class ACharacter;
class UPrimitiveComponent;

// This Class is used for handling collisions between multple characters
// and Static (Non-Movable) Meshes
// This class requires your character have
//      1. Trigger Capsule
//      2. four sockets to map out your characters directional state
//         They must match the names Front, Back, Left, Right
//      3. Your TriggerCapsule must have an OnComponentBeginOverlap & OnComponentEndOverlap
//         event to handle what goes on
//      4. This class requires you leave the default names of Mesh and/or StaticMesh,
//         for the component names
UCLASS()
class THEGAME_API UCollisionHandler : public UActorComponent
{
    GENERATED_BODY()
private:

    enum class ESocket
    {
        Front,
        Back,
        Left,
        Right
    };

    struct FCollision
    {
        bool Front = false;
        bool Back = false;
        bool Left = false;
        bool Right = false;
        
        bool AllClear() const;
        bool AllBlocked() const;
    };
    FCollision Collision;

    struct FMeshCompass
    {
        ACharacter* ThisCharacter = nullptr;
        const FName Front;
        const FName Back;
        const FName Left;
        const FName Right;
        FMeshCompass();
        FVector GetLocation(ESocket SocketName);
    };
    FMeshCompass MeshCompass;

    struct FColliding
    {
        AActor* Actor = nullptr;
        UPrimitiveComponent* Mesh = nullptr;
    };
    FColliding Self;
    FColliding Other;

    struct FEmptyObject
    {
        const FQuat Quat;
        const FCollisionShape Shape;
        FEmptyObject() : Quat(FQuat()), Shape(FCollisionShape::MakeSphere(55)) {}
    };
    FEmptyObject Empty;

    FRegexPattern* MeshStrPattern;

protected:

public:
    struct FMove
    {
        FVector Direction;
        float Value;
    };

    enum EDirectionals
    {
        ForwardBackward,
        LeftRight
    };
    UCollisionHandler();
    virtual ~UCollisionHandler();
    void Init(ACharacter* thisCharacter);

    USkeletalMeshComponent* GetMesh() const;
    ACharacter* GetThisCharacter() const;

    void SetCollidingActor(AActor* actor);
    AActor* GetCollidingActor() const;
    FVector GetCollisionLocation();

    bool AllClear() const;
    bool AllBlocked() const;
    void ModifyDirectional(const FVector& DirectionalRef, float X, float Y);
};
