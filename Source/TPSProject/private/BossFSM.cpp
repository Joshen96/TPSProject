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




