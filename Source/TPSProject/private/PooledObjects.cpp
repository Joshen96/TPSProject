// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledObjects.h"
#include "TimerManager.h"

APooledObjects::APooledObjects()
{
    PrimaryActorTick.bCanEverTick = false;
    Active = false;
    LifeSpan = 0.f;
    PoolIndex = -1;
}

void APooledObjects::Deactivate()
{
    SetActive(false);
    GetWorldTimerManager().ClearTimer(LifeSpanTimer);
    OnPooledObjectDespawn.Broadcast(this);
}

void APooledObjects::SetActive(bool IsActive)
{
    Active = IsActive;
    SetActorHiddenInGame(!IsActive);

    if (IsActive && LifeSpan > 0.f)
    {
        GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObjects::Deactivate, LifeSpan, false);
    }
}

void APooledObjects::SetLifeSpan(float LifeTime)
{
    LifeSpan = LifeTime;
}

void APooledObjects::SetPoolIndex(int index)
{
    PoolIndex = index;
}

bool APooledObjects::IsActive()
{
    return Active;
}

int APooledObjects::GetPoolIndex()
{
    return PoolIndex;
}

/*
// Sets default values
APooledObjects::APooledObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//OnPooledObjectDespawn.AddDynamic(this, &APooledObjects::Deactivate);

}

void APooledObjects::Deactivate()
{
	SetActive(false);
	

	GetWorldTimerManager().ClearAllTimersForObject(this);


	OnPooledObjectDespawn.Broadcast(this);
}

void APooledObjects::SetActive(bool IsActive)
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObjects::Deactivate, LifeSpan, false);
}

void APooledObjects::SetLifeSpan(float LifeTime)
{
	LifeSpan = LifeTime;
}

void APooledObjects::SetPoolIndex(int index)
{
	PoolIndex = index;
}

bool APooledObjects::IsActive()
{
	return Active;
}

int APooledObjects::GetPoolIndex()
{
	return PoolIndex;
}

*/