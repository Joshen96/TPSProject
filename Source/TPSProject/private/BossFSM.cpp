// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"

UBossFSM::UBossFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBossFSM::BeginPlay()
{
}

void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UEnemyFSM::TickComponent(DeltaTime, TickType, ThisTickFunction);

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




