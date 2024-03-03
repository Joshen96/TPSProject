// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledObjects.h"
#include "Components/ActorComponent.h"
#include "ObjectPools.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UObjectPools : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPools();


	//UFUNCTION(BlueprintCallable,Category ="ObjectPool")
	APooledObjects* SpawnPooledObject();

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	TSubclassOf<class APooledObjects> PooledObjectSubclass;

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int PoolSize = 20;

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	float PooledObjectLifeSpan = 0.0f;

	UFUNCTION()
	void OnPooledObjectDespawn(APooledObjects* PoolActor);



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TArray<APooledObjects*> ObjectPool;
	TArray<int32>SpawnedPoolIndexes;


		
};
