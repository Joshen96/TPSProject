// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"
#include "BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
/*
void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner(); //�ִ� �ν���Ʈ�� ���� ������

	// ������ ���� �÷��̾�� ĳ����
	auto Boss = Cast<ABossEnemy>(ownerPawn);

	FVector velocity = Boss->GetVelocity();
	//�÷��̾� ������� ���б�
	FVector forwardVector = Boss->GetActorForwardVector();
	//��������  speed�Ҵ��ϱ� 
	speed = FVector::DotProduct(forwardVector, velocity);

}
*/
void UBossAnim::OnEndAttackAnimation()
{
	bCloseAttack = false;
	bMiddleAttack = false;
	bDistanceAttack = false;
	
}
