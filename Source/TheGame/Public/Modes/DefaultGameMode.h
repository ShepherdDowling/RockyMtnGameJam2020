// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DefaultGameMode.generated.h"

class ARanger;
class ARangerPLC;
class APlayerController;
class AMobileCamera;
class UDefaultUI;
class UUserWidget;

/**
 * 
 */
UCLASS()
class THEGAME_API ADefaultGameMode : public AGameMode
{
	GENERATED_BODY()

private:
    ConstructorHelpers::FClassFinder<ACharacter>* BpRanger = nullptr;
    ConstructorHelpers::FClassFinder<APlayerController>* BpRangerPLC = nullptr;
    
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

    TArray<AActor*>  PlayerStartArr;
    TArray<ARanger*> RangerArr;

public:
    ADefaultGameMode();
    virtual ~ADefaultGameMode();
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    int GetMaxPlayerCount() const;
};
