// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SoundInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USoundInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TPSPROJECT_API ISoundInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    // Play a global sound
    virtual void PlayGlobalSound(const FString& SoundPath) = 0;

    // Play a sound at a specific location
    virtual void PlayLocationSound(USoundBase* Sound, FVector Location, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f) = 0;
};
