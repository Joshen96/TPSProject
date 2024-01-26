// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagneticItem.generated.h"

UCLASS()
class TPSPROJECT_API AMagneticItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagneticItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(Editanywhere, category=EXP)
	float Exp = 0;

	UPROPERTY(Editanywhere, Category = CollisionOut)
	//구 콜리션 컴포넌트
	class USphereComponent* collisionOut;

	UPROPERTY(Editanywhere, Category = CollisionInner)
	//구 콜리션 컴포넌트
	class USphereComponent* collisionInner;

};
