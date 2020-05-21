// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h" 
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathOnCharacterDelegate);

class UAnimate;
class UCollisionHandler;
class UPhysicsHandleComponent;

UCLASS()
class THEGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float HP = 1.0f;

	FDeathOnCharacterDelegate OnDeath;

	struct FHealth
	{
		UPROPERTY(EditDefaultsOnly, Category = "Setup")
		uint32 Starting = 100;

		UPROPERTY(VisibleAnywhere, Category = "Health")
		uint32 Current = 0;

		FHealth() { Current = Starting; }
	};

	FHealth Health;

	UPROPERTY(meta = (Native = "true")) // Turns Garbage Collection On
	TMap<FName, int32> OpponentDamageTrackMap;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UUserWidget* HUD;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCollisionHandler* cch = nullptr; // character collision handler

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAnimate* Animate = nullptr;


	ABaseCharacter();
	virtual ~ABaseCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected: // --------------------------- THESE PROPERTIES ARE PART OF UE4'S SKELETON SCRIPT ----------------
	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

	//** Follow camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	USceneComponent* SceneComponent = nullptr;
           // -------------------------- END OF UE4'S PROPERTIES -------------------------------------------
protected: // -------------------------- THESE FUNCTIONS ARE BY UE4'S SKELETON SCRIPT ----------------------
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	// Called via input to turn at a given rate. 
	// @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void TurnAtRate(float Rate);


	// Called via input to turn look up/down at a given rate. 
	// @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; /*Subobject*/ }
	//FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; /*Subobject*/ }

	// -------------------------- END OF UE4'S FUNCTIONS --------------------------------------------

public:
	int32 GetTotalAnimationsPlayed() const;

	FORCEINLINE void               SetHUD(UUserWidget* NewHUD) { HUD = NewHUD; }
	FORCEINLINE class UUserWidget* GetHUD() const { return HUD; }
	FORCEINLINE float              GetHP() const { return Health.Current * 0.01; }
};
