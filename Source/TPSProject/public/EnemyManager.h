// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"


UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//애너미 생성기 목표
	//얼마마다 적을 생성할것인지
	//어디에 생성할것인지 
	//최소 시간 최대시간 스폰액터 생성할 적 BP
	//생성함수
	
	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	float minTime = 1;

	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	float maxTime = 5;

	// 스폰액터위치
	
	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	TArray<class AActor*> spawnPoints; // 액터의 배열 

	// 생성할 에너미BP

	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	TSubclassOf<class AEnemy> enemyFactory; 

	//타이머 핸들 설정
	FTimerHandle  spawnTimerHandle;

	void CreatEnemy();

	UFUNCTION(BlueprintImplementableEvent, Category = Count)
	void CountEnemyEvent();
	

};
