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
	TSubclassOf<class AEnemy> enemyFactory; 

	//Ÿ�̸� �ڵ� ����
	FTimerHandle  spawnTimerHandle;

	void CreatEnemy();

	UFUNCTION(BlueprintImplementableEvent, Category = Count)
	void CountEnemyEvent();
	

};
