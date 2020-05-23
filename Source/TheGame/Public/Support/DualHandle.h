// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DualHandle.generated.h"

class UPhysicsHandleComponent;
class UPrimitiveComponent;

UCLASS()
class THEGAME_API ADualHandle : public AActor
{
	GENERATED_BODY()
	
private:	
	ACharacter* ThisCharacter = nullptr;
	UPrimitiveComponent* TargetComponent = nullptr;
	FName BoneName;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPhysicsHandleComponent* One = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPhysicsHandleComponent* Two = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float Friction = 9999999;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float GrabOffset = 50;


	virtual void BeginPlay() override;

public:
	ADualHandle();
	void Init(ACharacter* ThisCharacter, UPrimitiveComponent* TargetComponent, FName&& BoneName);
	virtual ~ADualHandle();
	virtual void Tick(float DeltaTime) override;


};
