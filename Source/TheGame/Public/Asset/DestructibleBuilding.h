// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.h"
#include "DestructibleBuilding.generated.h"

class FWatch;
class UDestructibleComponent;

/**
 * 
 */
UCLASS()
class THEGAME_API ADestructibleBuilding : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DamageTaken = 10;

	// Used for DamageRadius
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float WallStrength = 5; // Used for 

		// Used for DamageRadius
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DebrisTimeout = 50; // Used for 

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDestructibleComponent* DestructableComponent = nullptr;


	bool TimerBuildingCollapseStarted = false;
	bool PhysicsEnabled = true;
	int32 ExpCounter = 1;
	float BuildingHeight = 0;
	float BuidlingStartZ = 0;
	FVector LocationLastTick;
	FVector StartLocation;

protected:
	ADestructibleBuilding();
	virtual ~ADestructibleBuilding();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FWatch* Watch = nullptr;

public:
	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//UFUNCTION() // Don't use this below if you have OnCompHit
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
