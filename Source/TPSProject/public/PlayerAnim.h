// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	

	//��� ����Ʈ�� Event Blueprint Update Animation���� �����Ǵ� �Լ�
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//����� ���� ��
	//�ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim) //�ִϺ������Ʈ���� ������ ��밡���ϵ��� BlueprintReadWrite �� �������Ʈ���� ���������ϵ��� EditDefaultsOnly 
	float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim) //�ִϺ������Ʈ���� ������ ��밡���ϵ��� BlueprintReadWrite �� �������Ʈ���� ���������ϵ��� EditDefaultsOnly 
	float direction = 0;

	// �������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim) //�ִϺ������Ʈ���� ������ ��밡���ϵ��� BlueprintReadWrite �� �������Ʈ���� ���������ϵ��� EditDefaultsOnly 
	bool isInAir = false;


	//��Ÿ�� ����� �־������
	UPROPERTY(EditDefaultsOnly, Category=Player)
	class UAnimMontage* attckAnimMontage;

	void PlayAttckAnim();


};
