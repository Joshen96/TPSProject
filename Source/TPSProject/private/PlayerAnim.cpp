// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//�÷��̾��� �̵� �ӵ��� �����ͼ� speed�� �Ҵ��ϱ�
	//������ �� ��������
	auto ownerPawn = TryGetPawnOwner(); //�ִ� �ν���Ʈ�� ���� ������

	// ������ ���� �÷��̾�� ĳ����
	auto Player = Cast<ATPSPlayer>(ownerPawn);
	
	if (Player)//ĳ���� �����ϸ�
	{
		// �̵��ӵ� �ʿ�
		FVector velocity = Player->GetVelocity();
		//�÷��̾� ������� ���б�
		FVector forwardVector = Player->GetActorForwardVector();
		//��������  speed�Ҵ��ϱ� 
		speed = FVector::DotProduct(forwardVector,velocity);
	}
	
}
