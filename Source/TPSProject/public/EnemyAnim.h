// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h" //애너미 상태 받아오기위함
#include "BossFSM.h" 
#include "EnemyAnim.generated.h"


/**
 * 
 */
UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category=FSM)
	EEnemyState animstate; //상태확인용 나중에 동기화 해줘야함

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EBossState bossstate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category=FSM)
	bool bAttackPlay = false;

	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();

	//애니메이션 피격 재생 함수
	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent) // 선언없이 블루프린터에서 구현한다는 의미
	void PlayDamageAnim(FName sectionName);

};
