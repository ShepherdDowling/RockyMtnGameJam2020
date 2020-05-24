// Copyright by Shepherd Dowling under the Apache v2 licence

#include "Actor/BaseCharacter.h"
#include "Support/Animate.h"
#include "Support/CollisionHandler.h"
#include "Support/Rock.h"

#include "UObject/UObjectGlobals.h" 
#include "UObject/ConstructorHelpers.h" 
#include "Engine/World.h" 
#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h" 
#include "Components/ArrowComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Math/UnrealMathUtility.h" 


ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	/// If I set mesh outside the capsule I can get better collisions but can't move (which is worse)
	//SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	//SetRootComponent(SceneComponent);
	//GetCapsuleComponent()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	//GetMesh()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	//GetArrowComponent()->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	/// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	/// Create a follow camera (I'm not running this code because this game uses a shared camera)
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create Default Subobject requires a Component Type
	// It is used for creating a class that supports your base class
	// It should also not take up world space (for that use SpawnActor)
	Animate = CreateDefaultSubobject<UAnimate>(TEXT("Animator"));
	CollisionHandler = CreateDefaultSubobject<UCollisionHandler>(TEXT("Character Collision Handler"));
}

ABaseCharacter::~ABaseCharacter()
{

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	ensure(Animate);
	ensure(CollisionHandler);
	CollisionHandler->Init(this);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABaseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABaseCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABaseCharacter::OnResetVR);
}


void ABaseCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABaseCharacter::MoveForward(float Value)
{
	if (!Animate) return;
	if (!CollisionHandler) return;
	if (!Animate->RunningBlueprint()) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = FRotator(0, 0, 0); // Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (!CollisionHandler->GetCollidingActor())
			AddMovementInput(Direction, Value);
		else 
			CollisionHandler->ModifyDirectional(Direction, 0, Value); // X/Y axis
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (!Animate) return;
	if (!CollisionHandler) return;
	if (!Animate->RunningBlueprint()) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = FRotator(0, 0, 0); // Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); 
		// add movement in that direction (value -1 = left, value +1 = right (hence move right to what extent)

		if (!CollisionHandler->GetCollidingActor())
			AddMovementInput(Direction, Value);
		else {
			CollisionHandler->ModifyDirectional(Direction, Value, 0); // X/Y axis
		}
	}
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ABaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

int32 ABaseCharacter::GetTotalAnimationsPlayed() const
{
	return Animate->GetTotalAnimationsPlayed();
}


