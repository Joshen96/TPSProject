// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyFSM.h"
#include "BossFSM.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBossFSM : public UEnemyFSM
{
	GENERATED_BODY()
	

	UBossFSM();

protected:
	// Called when the game starts
	void BeginPlay() override;

public:
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AttckState() override;

	void IdleState() override;
	
	//필요속성 : 나의 위치

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ABossEnemy* me2;


};
