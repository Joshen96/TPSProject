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

    // ã�� �÷��̾ Ÿ�ٿ� ĳ����
    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());

    //�÷��̾� ���� ����ְ�
    target = Cast<ATPSPlayer>(actor);
    //�ֳʹ��� ���� ����ְ�  ���� �Ÿ������
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
        // ù ��° ���� ���� ����
        // ...
        break;
    case 2:
        // �� ��° ���� ���� ����
        // ...
        break;
    case 3:
        // �� ��° ���� ���� ����
        // ...
        break;
    default:
        // �⺻ ���� ���� ����
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

        //anim->animstate = mState;// move ���·� ��ȯ


        //GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos); 
    }
}


