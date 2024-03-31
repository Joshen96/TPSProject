// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.h"
#include "ItemBox.generated.h"

UCLASS()
class TPSPROJECT_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = Hitzone)
	class UBoxComponent* HitZone;
	
	UFUNCTION(BlueprintCallable, Category = Hit)
	virtual void OnHitProcess();

	UFUNCTION(BlueprintImplementableEvent, Category = Hit)
	void OnItemProcess(int set);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = SoundManager)
	ASoundManager* SoundManagerRef;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = SoundFile)
	FString SoundPath;
};
