// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset/FPSWidget.h"

#include "Slate/SObjectWidget.h"
#include "EngineGlobals.h"
#include "Templates/SharedPointer.h" 

// Build.cs add  "Slate", "SlateCore"

// this is how to pull in the global variable for the FPS
// lifted from PerformanceMonitor.cpp::Tick(), line 270
extern ENGINE_API float GAverageFPS;

void UFPSWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // enable tick for c++
    // disabled by default, unless animations / etc, see UserWidget.h:45
    TSharedPtr<SObjectWidget> SafeGCWidget = MyGCWidget.Pin();
    if (SafeGCWidget.IsValid())
        SafeGCWidget->SetCanTick(true);
}

void UFPSWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FPS = GAverageFPS;
}
