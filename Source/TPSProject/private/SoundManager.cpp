// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASoundManager::ASoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}



void ASoundManager::PlayGlobalSound(const FString& SoundPath)
{
	
	USoundBase* Sound = LoadObject<USoundBase>(nullptr, *SoundPath);
	if (Sound) 
	{
		UGameplayStatics::PlaySound2D(this, Sound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load sound at path: %s"), *SoundPath);
	}
}

void ASoundManager::PlayLocationSound(USoundBase* Sound, FVector Location, float VolumeMultiplier, float PitchMultiplier)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, Location, FRotator::ZeroRotator, VolumeMultiplier, PitchMultiplier);
}

