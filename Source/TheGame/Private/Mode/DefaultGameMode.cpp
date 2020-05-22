// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Mode/DefaultGameMode.h"
#include "Actor/Godzilla/Godzilla.h"
#include "Asset/MobileCamera.h"
#include "Support/Rock.h"
#include "Support/Watch.h"

#include "GameFramework/Actor.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Asset/DefaultHUD.h"
#include "UObject/UObjectGlobals.h" 
#include "Engine/World.h" 
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"



void ADefaultGameMode::SpawnPlayers()
{
    for (auto player : GodzillaArr)
        player->Destroy();
    GodzillaArr.Empty();
    
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArr);

    for (int32 vPlayerIdx = 0; vPlayerIdx < PlayerStartArr.Num() && vPlayerIdx < MaxPlayerCount; vPlayerIdx++)
    {
        APlayerController* vController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), vPlayerIdx);
        
        if (!vController)
            vController = UGameplayStatics::CreatePlayer(GetWorld(), vPlayerIdx, true); // true create controller

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
}

void ADefaultGameMode::LockSharedCamera()
{
    if (bUseSharedScreen)
    {
        FActorSpawnParameters vMobileCameraSpawnParams;
        vMobileCameraSpawnParams.Owner = this;

        auto vCamLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation() + FVector(-300, 0, 300);
        // minus to make the camera go back behind the player
        // 0 to stay inline with the player
        // plus to get an arial view of the player
        auto vCamRotation = FRotator(-40, 0, 0);
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
}

void ADefaultGameMode::AddHpBars()
{
    HUD = CreateWidget<UDefaultHUD>(GetWorld()->GetFirstPlayerController(), SubClassHUD);
    if (!ensure(HUD)) return;
    Cast<AGodzilla>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetHUD(HUD);
    HUD->AddToViewport();

    UpdateHPBars();
}

void ADefaultGameMode::Init()
{
    SpawnPlayers();
    LockSharedCamera();
    AddHpBars();
}

void ADefaultGameMode::UpdateHPBars()
{
    if (GodzillaArr.Num() < 2)
        return;

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
    static ConstructorHelpers::FClassFinder<UDefaultHUD> HPWidgetFinder(TEXT("/Game/_Assets/BpHPWidget"));
    if (!ensure(HPWidgetFinder.Class))
        return; //throw std::runtime_error("Game Couldn't Init BpHPWidget");

    DefaultPawnClass = nullptr;
    //PlayerControllerClass = BpGodzillaPLC->Class;
    
    SubClassHUD = HPWidgetFinder.Class;

    HPText.Add(TEXT("Player1HP"));
    HPText.Add(TEXT("Player2HP"));
    HPText.Add(TEXT("Player3HP"));
    HPText.Add(TEXT("Player4HP"));

    watch = new FWatch;
}

ADefaultGameMode::~ADefaultGameMode()
{
    if (BpGodzilla)
        delete BpGodzilla;

    if (BpGodzillaPLC)
        delete BpGodzillaPLC;

    if (watch)
        delete watch;
}

void ADefaultGameMode::StartPlay()
{
    Super::StartPlay();

    UE_LOG(LogTemp, Warning, TEXT("LOG: %s"), *GetWorld()->GetMapName());
    if (!bLevelLoaded)
        return;

    if (GetWorld()->GetMapName() != "UEDPIE_0_BattleGrounds")
    {
        SetActorTickEnabled(false);
        PrimaryActorTick.bCanEverTick = false;
        return;
    }
    Init();
}

void ADefaultGameMode::Tick(float DeltaSeconds)
{
    UpdateHPBars();
    if (!GameOver)
    {
        for (auto player : GodzillaArr)
        {
            if (player->GetHP() == 0)
            {
                GameOver = true;
                Winner = player->GetName();
                watch->SetTimer(4);
            }
        }
    }
    else if (GameOver && (!watch->TimerIsRunning()))
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
        RemoveFromRoot();
    }
}

int ADefaultGameMode::GetMaxPlayerCount() const
{
    return MaxPlayerCount;
}

bool ADefaultGameMode::GetLevelLoaded() const
{
    return bLevelLoaded;
}

void ADefaultGameMode::SetLevelLoaded(bool truth)
{
    bLevelLoaded = truth;
}
