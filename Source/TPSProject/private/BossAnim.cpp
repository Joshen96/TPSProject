// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnim.h"
#include "BossEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
/*
void UBossAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner(); //애니 인스턴트의 폰을 가져옴

	// 가져온 폰을 플레이어로 캐스팅
	auto Boss = Cast<ABossEnemy>(ownerPawn);

	FVector velocity = Boss->GetVelocity();
	//플레이어 전방백터 구학기
	FVector forwardVector = Boss->GetActorForwardVector();
	//내적으로  speed할당하기 
	speed = FVector::DotProduct(forwardVector, velocity);

}
*/
void UBossAnim::OnEndAttackAnimation()
{
	bCloseAttack = false;
	bMiddleAttack = false;
	bDistanceAttack = false;
	
}
