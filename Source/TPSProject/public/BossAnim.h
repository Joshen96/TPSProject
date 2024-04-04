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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = BossAnim) //�ִϺ������Ʈ���� ������ ��밡���ϵ��� BlueprintReadWrite �� �������Ʈ���� ���������ϵ��� EditDefaultsOnly 
	float speed = 0;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EBossState bossAnimState; //����Ȯ�ο� ���߿� ����ȭ �������

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay = false;

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndAttackAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent) // ������� ��������Ϳ��� �����Ѵٴ� �ǹ�
	void PlayDamageAnim(FName sectionName);
};
