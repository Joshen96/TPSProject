// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//플레이어의 이동 속도를 가져와서 speed에 할당하기
	//소유한 폰 가져오기
	auto ownerPawn = TryGetPawnOwner(); //애니 인스턴트의 폰을 가져옴

	// 가져온 폰을 플레이어로 캐스팅
	auto Player = Cast<ATPSPlayer>(ownerPawn);
	
	if (Player)//캐스팅 성공하면
	{
		// 이동속도 필요
		FVector velocity = Player->GetVelocity();
		//플레이어 전방백터 구학기
		FVector forwardVector = Player->GetActorForwardVector();
		//내적으로  speed할당하기 
		speed = FVector::DotProduct(forwardVector,velocity);
		
		//좌우 값 넣기
		FVector rightVector = Player->GetActorRightVector();

		direction = FVector::DotProduct(rightVector, velocity);
	
		// 땅에 섯는지 여부
		auto movement = Player->GetCharacterMovement();
		isInAir = movement->IsFalling();

	
	}
	
}
//공격시 몽타주실행 함수
void UPlayerAnim::PlayAttckAnim()
{
	Montage_Play(attckAnimMontage);
	//Montage_Play()몽타주 실행 함수 

}
