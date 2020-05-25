// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionHandler.generated.h"

class AActor;
class ACharacter;
class UPrimitiveComponent;

// This Class is used for handling collisions between multple characters
// and Static (Non-Movable) Meshes for SHARED SCREEN GAMES!!
// This class requires your character have
//      1. Your Camera be facing -Y direction
//      2. four sockets to map out your characters directional state
//         They must match the names Front, Back, Left, Right
//      3. Your Mesh must have an OnComponentBeginOverlap & OnComponentEndOverlap
//         event to handle what goes on
//      4. This class requires you leave the default component names of Mesh and/or StaticMesh,
UCLASS()
class THEGAME_API UCollisionHandler : public UActorComponent
{
    GENERATED_BODY()
private:
    struct FDirection
    {
        const FVector Vec;
        FDirection(FVector&& iVector);
        bool Up() const;
        bool Down() const;
        bool Left() const;
        bool Right() const;
        bool UpInPart() const;
        bool DownInPart() const;
        bool LeftInPart() const;
        bool RightInPart() const;
    };

    struct FMovement
    {
        ACharacter* ThisCharacter = nullptr;
        const FVector* DirectionalRefPtr = nullptr;
        const float X = 0;
        const float Y = 0;

        FMovement(ACharacter* iThisCharacter, const FVector* iDirectionalRefPtr, float iX, float iY);
        bool Up() const;
        bool Down() const;
        bool Left() const;
        bool Right() const;
        void Character();
    };

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

        void operator=(const FCollision& Other);
        bool AllClear() const;
        bool AllBlocked() const;
        FCollision Reverse() const;
    };
    
    FCollision Collision;
    FCollision Free;

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

    void ModifyDirectional(const FVector& DirectionalRef, float X, float Y);
};
