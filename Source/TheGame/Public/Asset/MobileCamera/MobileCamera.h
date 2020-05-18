// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MobileCamera.generated.h"

class AActor;
class USceneComponent;
class USpringArmComponent;
class UCameraComponent;

class AActor;
class ABaseCharacter;
class ADefaultGameMode;

/*
*  This Camera will work similar to the
*  Mario Bros camera because it will move
*  around as it follows the characters
*/
UCLASS()
class THEGAME_API AMobileCamera : public AActor
{
	GENERATED_BODY()
	
private:
	bool bInitComplete = false;
	ADefaultGameMode* GameModePtr		= nullptr;
	TArray<AActor*>*  PlayerStartsPtr	= nullptr;
	TArray<ABaseCharacter*>* BaseCharacterListPtr		= nullptr;

	FVector CurrentLocation;
	FVector TargetLocation;

	FTimerHandle TimerHandle;

	void UpdateSpringArmLength();
	void UpdateCameraLocation();
protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* DefaultScene = nullptr;

	// TODY: CHANGE BlueprintReadWrite TO VisibleAnywhere
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm = nullptr;

	// TODY: CHANGE BlueprintReadWrite TO VisibleAnywhere
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float CamerUpdateInSeconds = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinArmDistance = 700;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxArmDistance = 5000;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AMobileCamera();

	void Tick(float DeltaTime) override;
	void AddReferences(ADefaultGameMode* iGameModePtr, TArray<AActor*>* iPlayerStartsPtr, TArray<ABaseCharacter*>* iBaseCharacterListPtr);
	void LinkCameraAndActors();
	void FollowAndSpanPlayers();
};
