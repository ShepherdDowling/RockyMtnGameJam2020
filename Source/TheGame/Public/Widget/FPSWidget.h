// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSWidget.generated.h"

/**
 * From HORU Game
 */
UCLASS(BlueprintType, meta = (DontUseGenericSpawnObject = "True"))
class THEGAME_API UFPSWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // The FPS.
    UPROPERTY(BlueprintReadOnly)
    float FPS = -1;

    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};