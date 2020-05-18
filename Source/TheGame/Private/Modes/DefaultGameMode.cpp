// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Modes/DefaultGameMode.h"
#include "Support/Rock.h"
#include "Asset/MobileCamera/MobileCamera.h"

#include "Actor/Godzilla/Godzilla.h"

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
        UProgressBar* LifeBar = Cast<UProgressBar>(GodzillaArr[0]->GetHUD()->WidgetTree->FindWidget(HPText[i]));
        if (LifeBar)
            LifeBar->SetPercent(GodzillaArr[i]->GetHP());
    }
}

ADefaultGameMode::ADefaultGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    BpGodzilla = new ConstructorHelpers::FClassFinder<ACharacter>(TEXT("/Game/_Actors/Godzilla/BpGodzilla"));
    if (!(ensure(BpGodzilla) || ensure(BpGodzilla->Class)))
        return;  //throw std::runtime_error("Game Couldn't Init BpGodzilla");

    //BpGodzillaPLC = new ConstructorHelpers::FClassFinder<APlayerController>(TEXT("/Game/_Actors/Godzilla/BpGodzillaPLC"));
    //if (!(ensure(BpGodzillaPLC) || ensure(BpGodzillaPLC->Class)))
    //    return; //throw std::runtime_error("Game Couldn't Init BpGodzillaPLC");

    // Only here if you want to set defaults in the editor
    static ConstructorHelpers::FClassFinder<UDefaultUI> HPWidgetFinder(TEXT("/Game/_Assets/UI/BpHPWidget"));
    if (!ensure(HPWidgetFinder.Class))
        return; //throw std::runtime_error("Game Couldn't Init BpHPWidget");

    DefaultPawnClass = nullptr;
    //PlayerControllerClass = BpGodzillaPLC->Class;
    
    UIClass = HPWidgetFinder.Class;

    HPText.Add(TEXT("Player1HP"));
    HPText.Add(TEXT("Player2HP"));
    HPText.Add(TEXT("Player3HP"));
    HPText.Add(TEXT("Player4HP"));
}

ADefaultGameMode::~ADefaultGameMode()
{
    if (BpGodzilla)
        delete BpGodzilla;

    if (BpGodzillaPLC)
        delete BpGodzillaPLC;
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
            FActorSpawnParameters vGodzillaSpawnParams;
            vGodzillaSpawnParams.Owner = vController;

            AGodzilla* vGodzilla = GetWorld()->SpawnActor<AGodzilla>(
                BpGodzilla->Class,
                PlayerStartArr[vPlayerIdx]->GetActorLocation(),
                PlayerStartArr[vPlayerIdx]->GetActorRotation(),
                vGodzillaSpawnParams
                );

            vController->Possess(vGodzilla);
        }
        GodzillaArr.Add(Cast<AGodzilla>(vController->GetCharacter()));
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
            MobileCamera->AddReferences(this, &PlayerStartArr, &GodzillaArr);
            MobileCamera->LinkCameraAndActors();
        }
    }

    DefaultUI = CreateWidget<UDefaultUI>(GetWorld()->GetFirstPlayerController(), UIClass);
    if (!ensure(DefaultUI)) return;
    Cast<AGodzilla>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetHUD(DefaultUI);
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
