// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseCharacter.h"
#include "Godzilla.generated.h"


class UDefaultUI;
class UAnimSequence;
class UAnimate;
class UCapsuleComponent;
class UStaticMeshComponent;
class UPhysicsHandleComponent;
class UPrimitiveComponent;
class USceneComponent;
class UDualHandle;

#define AnimatorBP ConstructorHelpers::FObjectFinder<UAnimSequence>


UCLASS(config = Game)
class AGodzilla : public ABaseCharacter
{
	GENERATED_BODY()
private:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneComponent* Scene = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDualHandle* DualHandle = nullptr;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	AGodzilla();
	virtual ~AGodzilla();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void FaceLock();
	void RightTailWhip();
	void Bite();

	UFUNCTION()
	void OnCompBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnCompEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};

