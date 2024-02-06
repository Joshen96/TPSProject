// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyDataAsset.generated.h"

/**
 * 
 */


USTRUCT()
struct FMySkillinfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString SkillName;

	UPROPERTY(EditAnywhere)
	FString SKillExplanation;

	UPROPERTY(EditAnywhere)
	UTexture2D* Skillicon;

	UPROPERTY(EditAnywhere)
	int32 SkillLevel;

};




UCLASS()
class TPSPROJECT_API UMyDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FMySkillinfo> AssetItems;
};
