// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPools.h"
#include "Bullet_OP.h"
#include <GameFramework/ProjectileMovementComponent.h>

UObjectPools::UObjectPools()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UObjectPools::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; i++)
			{
				APooledObjects* PoolableActor = World->SpawnActor<APooledObjects>(PooledObjectSubclass, FVector::ZeroVector, FRotator::ZeroRotator);
				if (PoolableActor != nullptr)
				{
					PoolableActor->SetActive(false);
					PoolableActor->SetPoolIndex(i);
					PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPools::OnPooledObjectDespawn);
					ObjectPool.Add(PoolableActor);
				}
			}
		}
	}
}

APooledObjects* UObjectPools::SpawnPooledObject()
{
	for (APooledObjects* PoolableActor : ObjectPool)
	{
		if (PoolableActor != nullptr && !PoolableActor->IsActive())
		{
			PoolableActor->TeleportTo(FVector::ZeroVector, FRotator::ZeroRotator);
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
			return PoolableActor;
		}
	}

	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.RemoveAt(0);
		APooledObjects* PoolableActor = ObjectPool[PooledObjectIndex];
		if (PoolableActor != nullptr)
		{
			PoolableActor->SetActive(false);
			PoolableActor->TeleportTo(FVector::ZeroVector, FRotator::ZeroRotator);
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PooledObjectIndex);
			return PoolableActor;
		}
	}

	return nullptr;
}

void UObjectPools::OnPooledObjectDespawn(APooledObjects* PoolActor)
{
	SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}



/*
// Sets default values for this component's properties
UObjectPools::UObjectPools()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
void UObjectPools::BeginPlay()
{
	Super::BeginPlay();
	if (PooledObjectSubclass != nullptr) {
		UWorld* const World = GetWorld();

		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; i++)
			{
				APooledObjects* PoolableActor = World->SpawnActor<APooledObjects>(PooledObjectSubclass, FVector().ZeroVector, FRotator().ZeroRotator);
			
				

				if (PoolableActor != nullptr)
				{
					PoolableActor->SetActive(false);
					PoolableActor->SetPoolIndex(i);
					PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPools::OnPooledObjectDespawn);
					ObjectPool.Add(PoolableActor);
				}
			}
		}
	}

}

APooledObjects* UObjectPools::SpawnPooledObject()
{
	
	for (APooledObjects* PoolableActor : ObjectPool) {
		if (PoolableActor != nullptr && !PoolableActor->IsActive())
		{
			PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
			
			//auto Bulletspeed = Cast<UProjectileMovementComponent>(PoolableActor);
			
			//Bulletspeed->SimulationEnabled(true);
			//auto BulletActive = Cast<UProjectileMovementComponent>(Bulletspeed);

			//Bulletspeed->bAutoActivate = true;
			//auto Bulletprojectile = Cast<UProjectileMovementComponent>(Bulletspeed);
			//Bulletprojectile->bSimulationEnabled = true;
			
			return PoolableActor;

		}
	}
	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		APooledObjects* PoolableActor = ObjectPool[PooledObjectIndex];

		if (PoolableActor != nullptr)
		{
			PoolableActor->SetActive(false);

			PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableActor->GetPoolIndex());
			auto Bulletspeed = Cast<ABullet_OP>(PoolableActor);

			//Bulletspeed->SimulationEnabled(true);


			return PoolableActor;
		}
	}

	return nullptr;
}



// Called when the game starts

void UObjectPools::OnPooledObjectDespawn(APooledObjects* PoolActor)
{
	SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}


*/