// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

class AGodzilla;
class ABaseCharacter;
class APlayerController;
class AMobileCamera;
class UDefaultHUD;
class UUserWidget;
class FWatch;

/**
 * 
 */
UCLASS()
class THEGAME_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()

private:
    ConstructorHelpers::FClassFinder<ACharacter>* BpGodzilla = nullptr;
    ConstructorHelpers::FClassFinder<APlayerController>* BpGodzillaPLC = nullptr;
    
    AMobileCamera* MobileCamera = nullptr;
    UUserWidget* HUD = nullptr;

    TArray<FName> HPText;

    void SetSimulateGame();
    void UpdateHPBars();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    int MaxPlayerCount = 2;

    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    bool bUseSharedScreen = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<class UDefaultHUD> SubClassHUD;

    FWatch* watch;
    TArray<AActor*>  PlayerStartArr;
    TArray<ABaseCharacter*> PlayerPawnArr;

    bool bLevelLoaded = false;
    bool bSimulatingGame = false;

    bool SpawnPlayers();
    void LockSharedCamera();
    void AddHpBars();

public:
    void Init();
    ADefaultGameMode();
    virtual ~ADefaultGameMode();
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    int GetMaxPlayerCount() const;
    bool GetLevelLoaded() const;
    void SetLevelLoaded(bool truth); 
    TArray<ABaseCharacter*> GetPlayerPawnArray() const;
};
