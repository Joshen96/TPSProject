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


	//�ֳʹ� ������ ��ǥ
	//�󸶸��� ���� �����Ұ�����
	//��� �����Ұ����� 
	//�ּ� �ð� �ִ�ð� �������� ������ �� BP
	//�����Լ�
	
	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	float minTime = 1;

	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	float maxTime = 5;

	// ����������ġ
	
	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	TArray<class AActor*> spawnPoints; // ������ �迭 

	// ������ ���ʹ�BP

	UPROPERTY(EditAnyWhere, Category = SpawnSetting)
	TArray<TSubclassOf<class AEnemy>> enemyFactory;

	//Ÿ�̸� �ڵ� ����
	FTimerHandle  spawnTimerHandle;

	// �������Ʈ���� ���ε��� �̺�Ʈ �Լ� ����
	UFUNCTION(BlueprintCallable, Category = Count)
	void CreatEnemy();

	// �������Ʈ���� ���ε��� �̺�Ʈ ��������Ʈ ����
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SpawnSetting)
	bool bSpawnLock = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SpawnSetting)
	int32 SpawnProbability = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SpawnSetting)
	bool bDistanceEnemySpawn = false;

	UFUNCTION(BlueprintImplementableEvent, Category = Count)
	void CountEnemyEvent(AEnemy *EnemyOBJ);
	

};
