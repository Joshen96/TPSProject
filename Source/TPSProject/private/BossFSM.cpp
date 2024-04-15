// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "TPSPlayer.h"
#include "BossEnemy.h"
#include "TPSProject.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "BossAnim.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"



UBossFSM::UBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBossFSM::BeginPlay()
{
    
	Super::BeginPlay();


    currntHp = maxHp;

    // 찾은 플레이어를 타겟에 캐스팅
    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());

    //플레이어 액터 담아주고
    target = Cast<ATPSPlayer>(actor);
    //애너미의 액터 담아주고  이제 거리계산함
    me = Cast<ABossEnemy>(GetOwner());

    mState = EBossState::Idel;

	bossAnim = Cast<UBossAnim>(me->GetMesh()->GetAnimInstance());



}

void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    switch (mState)
    {
    case EBossState::Idel:
        IdleState();
        break;
    case EBossState::Move:
        MoveState();
        break;
    case EBossState::Attck:
        AttckState();
        break;
    case EBossState::Damage:
        DamageState();
        break;
    case EBossState::Die:
        DieState();
        break;

    }


}

void UBossFSM::AttckState()
{
	//me->Jump();

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());// 타겟과 거리 계산

	currentTime += GetWorld()->DeltaTimeSeconds;
	

	if (currentTime > attackDelayTime) {
		//PRINT_LOG(TEXT("Attack!!"));
		currentTime = 0;

		if (distance >= distance_attackRangeStart && distance < distance_attacRangekEnd) //원거리 공격
		{
			currentTime = 0;
			

			bossAnim->bDistanceAttack = true;
			

		}
		if (distance > close_attackRangeEnd && distance < distance_attackRangeStart&& bossAnim->bDistanceAttack==false) // 중거리 공격
		{
			
			currentTime = 0;
			
			bossAnim->bMiddleAttack = true;
			
		}
		if (distance > close_attackRangeStart && distance < close_attackRangeEnd)	// 근거리 공격
		{
			currentTime = 0;
			

			bossAnim->bCloseAttack = true;
			
		}
	}



	


	


	

	if (distance > attackedRange)
	{
		//me->Attackend();
		
		mState = EBossState::Move;
		currentTime = 0;
		bossAnim->bossAnimState = mState;

	}

}


void UBossFSM::IdleState()
{
	//me->GetCapsuleComponent()->SetSimulatePhysics(false);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelyTime) {
		mState = EBossState::Move;
		currentTime = 0;

		bossAnim->bossAnimState = mState;// move 상태로 변환

	}
}

void UBossFSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("sad"));
	//1.타겟 목적지가 필요
	FVector destination = target->GetActorLocation();
	//2.방향 필요  =  목적지 - 내위치 = 방향
	FVector dir = destination - me->GetActorLocation();
	//3.방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal());
	//me->GetCharacterMovement()->MaxWalkSpeed = 12000.0f;

	

	if (dir.Size() < 1500)
	{

		//ai->StopMovement();

		mState = EBossState::Attck;
		bossAnim->bossAnimState = mState;

		//anim->bAttackPlay = true;

		currentTime = attackDelayTime;
	}


}
/*
void UBossFSM::AttckState()
{
	//시간을 주어 공격 딜레이
	currentTime += GetWorld()->DeltaTimeSeconds;

	//공격시 타겟을 바라보도록 설정
	FRotator Lookat = UKismetMathLibrary::FindLookAtRotation(me->GetActorLocation(), target->GetActorLocation());
	Lookat.Pitch = 0; //위아래 잠금 pitch 회전잠금
	me->SetActorRotation(Lookat);
	me->Attackstart();

	if (currentTime > attackDelayTime) {
		//PRINT_LOG(TEXT("Attack!!"));


		currentTime = 0;

		anim->bAttackPlay = true;

	}


	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	if (distance > attackedRange)
	{
		me->Attackend();
		mState = EBossState::Move;
		anim->bossstate = mState;
	
	}


}
*/
//데미지 입은 상태
void UBossFSM::DamageState()
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EBossState::Idel;
		currentTime = 0;

		bossAnim->bossAnimState = mState;
		//me->GetCapsuleComponent()->SetSimulatePhysics(false);
	}
}
//죽음상태

void UBossFSM::DieState()
{
	
	me->Die();


	


}
// 대미지 입을때 맞은곳 파라매터
void UBossFSM::OnDamageProcess(int _damagehp)
{
	//데미지 이미지 출력하기
	me->CreateDamageUI(_damagehp);
	me->SettingHPUI();

	



	me->Hit();
	currntHp -= _damagehp;
	//ai->StopMovement();

	
	


	if (currntHp > 0)
	{
		if (mState == EBossState::Die)
		{
			//me->CreateDamageUI(_damagehp);
			return;
		}
		if (currntHp <= 4000 && !health4000)
		{
			health4000 = true;
			//피격
			mState = EBossState::Damage;

			//피격 애니메이션
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);

			
		}
		if (currntHp <= 3000 && !health3000)
		{
			health3000 = true;
			//피격
			mState = EBossState::Damage;

			//피격 애니메이션
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);
		}
		if (currntHp <= 2000 && !health2000)
		{
			health2000 = true;
			//피격
			mState = EBossState::Damage;

			//피격 애니메이션
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);
		}
		if (currntHp <= 1000 && !health1000)
		{
			health1000 = true;
			//피격
			mState = EBossState::Damage;

			//피격 애니메이션
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);
		}





	


	}
	else
	{
	
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EBossState::Die;


		//me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//me->GetMesh()->SetSimulatePhysics(true);



	}
	bossAnim->bossAnimState = mState;






}





