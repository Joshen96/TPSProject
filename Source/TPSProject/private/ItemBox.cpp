// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "Components/BoxComponent.h"
// Sets default values
AItemBox::AItemBox()
{
	HitZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit"));

	HitZone->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnOverlapBegin);
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
	int32 RandomSet = FMath::RandRange(0, 2);
	UE_LOG(LogTemp, Warning, TEXT("Boxtouch %d"),RandomSet);
	OnItemProcess(RandomSet);
	
}

void AItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnHitProcess();
	//SoundManagerRef->PlayGlobalSound(SoundPath);
}






