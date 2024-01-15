// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PooledObjects.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawn, APooledObjects*, PoolActor);



UCLASS()
class TPSPROJECT_API APooledObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledObjects();

	FOnPooledObjectDespawn OnPooledObjectDespawn;



	UFUNCTION(BlueprintCallable, Category = PooledObject)
	
	virtual	void Deactivate();

	void SetActive(bool IsActive);
	void SetLifeSpan(float LifeTime);
	void SetPoolIndex(int index);

	bool IsActive();
	int GetPoolIndex();

protected:


	bool Active;
	float LifeSpan = 0.0f;
	int PoolIndex;


	FTimerHandle LifeSpanTimer;



};
