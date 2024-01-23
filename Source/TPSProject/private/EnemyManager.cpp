// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h" // ���� �����ϱ����� ����

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Ÿ�� ���� ������ ����
	float createTime = FMath::RandRange(minTime, maxTime);

	// Ÿ�̸� �����ϱ�  ���忡 �̹� �����ϴ� �̱��� Ÿ�̸ӸŴ������� SetTimer�Լ��� ���
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreatEnemy, createTime);

	
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreatEnemy()
{
	// ��ġ�� ���ϰ�
	int index = FMath::RandRange(0, spawnPoints.Num() - 1); // �迭��.Num()��  int�� ���� ��ȯ����
	// ���� ��ġ�� ����
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, spawnPoints[index]->GetActorLocation(), FRotator(0));
	CountEnemyEvent();

	float createTime = FMath::RandRange(minTime, maxTime);

	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreatEnemy, createTime);
	//������ �ð� Ÿ�̸� ����
}

