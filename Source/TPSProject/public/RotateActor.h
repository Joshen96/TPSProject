// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateActor.generated.h"

UCLASS()
class TPSPROJECT_API ARotateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotateActor();

	float AngleAxis;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Editanywhere, Category = Movement)
	FVector Dimensions;

	UPROPERTY(Editanywhere, Category = Movement)
	FVector AxisVector;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = Movement)
	float Multiplier;

	UPROPERTY(Editanywhere,BlueprintReadWrite, Category = Damage)
	float DamageHoming;


};
