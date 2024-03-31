// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h" // 적을 생성하기위함 컴포

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
	
	//타임 간격 랜덤값 생성
	float createTime = FMath::RandRange(minTime, maxTime);

	// 타이머 설정하기  월드에 이미 존재하는 싱글턴 타이머매니저에서 SetTimer함수로 사용
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreatEnemy, createTime);

	
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyManager::CreatEnemy()
{
	if (bSpawnLock == true) 
	{

		float createTime = FMath::RandRange(minTime, maxTime);

		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreatEnemy, createTime);
	}
	else
	{
		// 스폰 위치를 정하고
		int index = FMath::RandRange(0, spawnPoints.Num() - 1); // 배열의.Num()는  int형 갯수 반환해줌
		
		// 적을 위치에 생성
		if (bDistanceEnemySpawn == false) {

			CountEnemyEvent1(GetWorld()->SpawnActor<AEnemy>(enemyFactory[0], spawnPoints[index]->GetActorLocation(), FRotator(0)));
		}
		else//원거리적 소환
		{	// 기본값 25% 
			int32 RandomNumber = FMath::RandRange(0, 99); 

			if (RandomNumber < SpawnProbability) // 25%
			{
				CountEnemyEvent1(GetWorld()->SpawnActor<AEnemy>(enemyFactory[1], spawnPoints[index]->GetActorLocation(), FRotator(0)));
			}
			else
			{
				CountEnemyEvent1(GetWorld()->SpawnActor<AEnemy>(enemyFactory[0], spawnPoints[index]->GetActorLocation(), FRotator(0)));
			}

		}

		//다시 시간 설정
		float createTime = FMath::RandRange(minTime, maxTime);

		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AEnemyManager::CreatEnemy, createTime);
		//랜덤을 시간 타이머 설정
	}

}

