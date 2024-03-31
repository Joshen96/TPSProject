// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h" //추가
#include "SoundInterface.h" //추가
#include "SoundManager.generated.h"

UCLASS()
class TPSPROJECT_API ASoundManager : public AActor, public ISoundInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	

	virtual void PlayGlobalSound(const FString& SoundPath) override;

	virtual void PlayLocationSound(USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f) override;
};
