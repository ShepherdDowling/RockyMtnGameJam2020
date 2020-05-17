// Fill out your copyright notice in the Description page of Project Settings.


#include "Modes/DefaultGameMode.h"
#include "Support/Rock.h"
#include "Asset/MobileCamera/MobileCamera.h"

#include "Actor/Ranger/Ranger.h"
#include "Actor/Ranger/RangerPLC.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "Widget/DefaultUI.h"

#include "UObject/UObjectGlobals.h" 
#include "Engine/World.h" 
#include "GameFramework/Actor.h" 
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void ADefaultGameMode::UpdateHPBars()
{
    for (int32 i = 0; i < MaxPlayerCount; i++)
    {
        UProgressBar* LifeBar = Cast<UProgressBar>(RangerArr[0]->GetHUD()->WidgetTree->FindWidget(HPText[i]));
        if (LifeBar)
            LifeBar->SetPercent(RangerArr[i]->GetHP());
    }
}

ADefaultGameMode::ADefaultGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    BpRanger = new ConstructorHelpers::FClassFinder<ACharacter>(TEXT("/Game/_Actors/Ranger/BpRanger"));
    if (!(ensure(BpRanger) || ensure(BpRanger->Class)))
        return;  //throw std::runtime_error("Game Couldn't Init BpRanger");

    BpRangerPLC = new ConstructorHelpers::FClassFinder<APlayerController>(TEXT("/Game/_Actors/Ranger/BpRangerPLC"));
    if (!(ensure(BpRangerPLC) || ensure(BpRangerPLC->Class)))
        return; //throw std::runtime_error("Game Couldn't Init BpRangerPLC");

    // Only here if you want to set defaults in the editor
    static ConstructorHelpers::FClassFinder<UDefaultUI> HPWidgetFinder(TEXT("/Game/_Assets/UI/BpHPWidget"));
    if (!ensure(HPWidgetFinder.Class))
        return; //throw std::runtime_error("Game Couldn't Init BpHPWidget");

    DefaultPawnClass = nullptr;
    PlayerControllerClass = BpRangerPLC->Class;
    
    UIClass = HPWidgetFinder.Class;

    HPText.Add(TEXT("Player1HP"));
    HPText.Add(TEXT("Player2HP"));
    HPText.Add(TEXT("Player3HP"));
    HPText.Add(TEXT("Player4HP"));
}

ADefaultGameMode::~ADefaultGameMode()
{
    if (BpRanger)
        delete BpRanger;

    if (BpRangerPLC)
        delete BpRangerPLC;
}

void ADefaultGameMode::StartPlay()
{
    Super::StartPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArr);

    for (int32 vPlayerIdx = 0; vPlayerIdx < PlayerStartArr.Num() && vPlayerIdx < MaxPlayerCount; vPlayerIdx++)
    {
        APlayerController* vController = Statics::GetPlayerControllerFromID(GetWorld(), vPlayerIdx);
        if (!vController)
            vController = Statics::CreatePlayer(GetWorld(), vPlayerIdx, true); // true create controller

        if (!ensure(vController))
            continue;

        if (!vController->GetPawn())
        {
            FActorSpawnParameters vRangerSpawnParams;
            vRangerSpawnParams.Owner = vController;

            ARanger* vRanger = GetWorld()->SpawnActor<ARanger>(
                BpRanger->Class,
                PlayerStartArr[vPlayerIdx]->GetActorLocation(),
                PlayerStartArr[vPlayerIdx]->GetActorRotation(),
                vRangerSpawnParams
                );

            vController->Possess(vRanger);
        }
        RangerArr.Add(Cast<ARanger>(vController->GetCharacter()));
    }

    if (bUseSharedScreen)
    {
        FActorSpawnParameters vMobileCameraSpawnParams;
        vMobileCameraSpawnParams.Owner = this;

        auto vCamLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation() + FVector(-300, 0, 300);
        // minus to make the camera go back behind the player
        // 0 to stay inline with the player
        // plus to get an arial view of the player
        auto vCamRotation = FRotator(-45, 0, 0);
        // X = Pitch = roller coaster
        // Y = Roll  = Clock
        // Z = Yaw   = left/right

        MobileCamera = GetWorld()->SpawnActor<AMobileCamera>(AMobileCamera::StaticClass(),
            vCamLocation, vCamRotation, vMobileCameraSpawnParams);

        if (ensure(MobileCamera))
        {
            MobileCamera->AddReferences(this, &PlayerStartArr, &RangerArr);
            MobileCamera->LinkCameraAndActors();
        }
    }

    DefaultUI = CreateWidget<UDefaultUI>(GetWorld()->GetFirstPlayerController(), UIClass);
    if (!ensure(DefaultUI)) return;
    Cast<ARanger>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetHUD(DefaultUI);
    DefaultUI->AddToViewport();

    UpdateHPBars();
}

void ADefaultGameMode::Tick(float DeltaSeconds)
{
    UpdateHPBars();
}

int ADefaultGameMode::GetMaxPlayerCount() const
{
    return MaxPlayerCount;
}
