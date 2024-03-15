// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "Components/BoxComponent.h"
// Sets default values
AItemBox::AItemBox()
{
	HitZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit"));


}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBox::OnHitProcess()
{
	int32 RandomSet = FMath::RandRange(0, 3);
	UE_LOG(LogTemp, Warning, TEXT("Boxtouch %d"),RandomSet);
	OnItemProcess(RandomSet);

}





