// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BossEnemy.h"
#include "TPSPlayer.h"


UBossFSM::UBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBossFSM::BeginPlay()
{
    currntHp = maxHp;

    // 찾은 플레이어를 타겟에 캐스팅
    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());

    //플레이어 액터 담아주고
    target = Cast<ATPSPlayer>(actor);
    //애너미의 액터 담아주고  이제 거리계산함
    me2 = Cast<ABossEnemy>(GetOwner());

    mState = EEnemyState::Idel;

    anim = Cast<UEnemyAnim>(me2->GetMesh()->GetAnimInstance());



}

void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    switch (mState)
    {
    case EEnemyState::Idel:
        IdleState();
        break;
    case EEnemyState::Move:
        MoveState();
        break;
    case EEnemyState::Attck:
        AttckState();
        break;
    case EEnemyState::Damage:
        DamageState();
        break;
    case EEnemyState::Die:
        DieState();
        break;

    }


}

void UBossFSM::AttckState()
{
    int32 AttackType = FMath::RandRange(1, 3);

    switch (AttackType)
    {
    case 1:
        // 첫 번째 공격 패턴 실행
        // ...
        break;
    case 2:
        // 두 번째 공격 패턴 실행
        // ...
        break;
    case 3:
        // 세 번째 공격 패턴 실행
        // ...
        break;
    default:
        // 기본 공격 패턴 실행
        // ...
        break;
    }
}

void UBossFSM::IdleState()
{
    //me->GetCapsuleComponent()->SetSimulatePhysics(false);
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime > idleDelyTime) {
        mState = EEnemyState::Move;
        currentTime = 0;

        //anim->animstate = mState;// move 상태로 변환


        //GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos); 
    }
}


