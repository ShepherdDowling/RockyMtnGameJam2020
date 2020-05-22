// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

class AGodzilla;
class ABaseCharacter;
class APlayerController;
class AMobileCamera;
class UDefaultUI;
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
    UUserWidget* DefaultUI = nullptr;

    TArray<FName> HPText;

    void UpdateHPBars();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    int MaxPlayerCount = 2;

    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    bool bUseSharedScreen = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<class UDefaultUI> UIClass;

    bool GameOver = false;
    FWatch* watch;
    TArray<AActor*>  PlayerStartArr;
    TArray<ABaseCharacter*> GodzillaArr;

    FString Winner;
    bool bLevelLoaded = false;
    void SpawnPlayers();
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
};
