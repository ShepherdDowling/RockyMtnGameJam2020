// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Mode/DefaultGameMode.h"
#include "Actor/Godzilla/Godzilla.h"
#include "Asset/MobileCamera.h"
#include "Asset/DefaultHUD.h"
#include "Support/Watch.h"
#include "Support/StaticData.h"

#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Controller.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"



bool ADefaultGameMode::SpawnPlayers()
{
    for (auto player : PlayerPawnArr)
        player->Destroy();
    PlayerPawnArr.Empty();

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
        PlayerPawnArr.Add(Cast<AGodzilla>(vController->GetCharacter()));
    }

    if (!GetWorld()->GetFirstPlayerController())
        return true;
    if (!GetWorld()->GetFirstPlayerController()->GetPawn())
        return true;

    return false;
}

void ADefaultGameMode::LockSharedCamera()
{
    if (bUseSharedScreen)
    {
        FActorSpawnParameters vMobileCameraSpawnParams;
        vMobileCameraSpawnParams.Owner = this;

        auto vCamLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation();
        // minus to make the camera go back behind the player
        // 0 to stay inline with the player
        // plus to get an arial view of the player
        auto vCamRotation = FRotator(0, 0, -35); // Look down at a 35 degree angle

        MobileCamera = GetWorld()->SpawnActor<AMobileCamera>(AMobileCamera::StaticClass(),
            vCamLocation, vCamRotation, vMobileCameraSpawnParams);

        if (ensure(MobileCamera))
        {
            MobileCamera->AddReferences(this, &PlayerStartArr, &PlayerPawnArr);
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
    bSimulatingGame = SpawnPlayers();

    if (bSimulatingGame)
        return SetSimulateGame();

    LockSharedCamera();
    AddHpBars();
}

void ADefaultGameMode::SetSimulateGame()
{
    bSimulatingGame = true;
    SetActorTickEnabled(false);
    PrimaryActorTick.bCanEverTick = false;
}

void ADefaultGameMode::UpdateHPBars()
{
    if (PlayerPawnArr.Num() < 2)
        return;

    for (int32 i = 0; i < MaxPlayerCount; i++)
    {
        UProgressBar* LifeBar = Cast<UProgressBar>(PlayerPawnArr[0]->GetHUD()->WidgetTree->FindWidget(HPText[i]));
        if (LifeBar && PlayerPawnArr[i])
            LifeBar->SetPercent(PlayerPawnArr[i]->GetHP());
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

    AStaticData::Winner = 0;
    AStaticData::GameOver = false;


    UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());
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
    if (!AStaticData::GameOver)
    {
        int inc = 0;
        for (auto player : PlayerPawnArr)
        {
            inc++;
            if (player->GetHP() == 0)
            {
                AStaticData::GameOver = true;
                watch->SetTimer(4);
            }
        }
    }
    else if (AStaticData::GameOver && (!watch->TimerIsRunning()))
    {
        int inc = 0;
        for (auto player : PlayerPawnArr)
        {
            inc++;
            if (player->GetHP() > 0)
                AStaticData::Winner = inc;
        }
        AStaticData::GameOver = true;
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

TArray<ABaseCharacter*> ADefaultGameMode::GetPlayerPawnArray() const
{
    return PlayerPawnArr;
}

