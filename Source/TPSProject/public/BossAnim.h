// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFSM.h"
#include "BossAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBossAnim : public UAnimInstance
{
	GENERATED_BODY()
public:

	//virtual void NativeUpdateAnimation(float DeltaSeconds) override; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BossAnim) //애니블부프린트에서 가져와 사용가능하도록 BlueprintReadWrite 와 블루프린트에서 편집가능하도록 EditDefaultsOnly 
	float speed = 0;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EBossState bossAnimState; //상태확인용 나중에 동기화 해줘야함

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay = false;

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndAttackAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent) // 선언없이 블루프린터에서 구현한다는 의미
	void PlayDamageAnim(FName sectionName);
};
