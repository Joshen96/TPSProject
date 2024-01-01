// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h" //�ֳʹ� ���� �޾ƿ�������
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
	EEnemyState animstate; //����Ȯ�ο� ���߿� ����ȭ �������
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category=FSM)
	bool bAttackPlay = false;

	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();
};
