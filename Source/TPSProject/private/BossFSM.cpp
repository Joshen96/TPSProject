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

    // ã�� �÷��̾ Ÿ�ٿ� ĳ����
    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());

    //�÷��̾� ���� ����ְ�
    target = Cast<ATPSPlayer>(actor);
    //�ֳʹ��� ���� ����ְ�  ���� �Ÿ������
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

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());// Ÿ�ٰ� �Ÿ� ���

	currentTime += GetWorld()->DeltaTimeSeconds;
	

	if (currentTime > attackDelayTime) {
		//PRINT_LOG(TEXT("Attack!!"));
		currentTime = 0;

		if (distance >= distance_attackRangeStart && distance < distance_attacRangekEnd) //���Ÿ� ����
		{
			currentTime = 0;
			

			bossAnim->bDistanceAttack = true;
			

		}
		if (distance > close_attackRangeEnd && distance < distance_attackRangeStart&& bossAnim->bDistanceAttack==false) // �߰Ÿ� ����
		{
			
			currentTime = 0;
			
			bossAnim->bMiddleAttack = true;
			
		}
		if (distance > close_attackRangeStart && distance < close_attackRangeEnd)	// �ٰŸ� ����
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

		bossAnim->bossAnimState = mState;// move ���·� ��ȯ

	}
}

void UBossFSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("sad"));
	//1.Ÿ�� �������� �ʿ�
	FVector destination = target->GetActorLocation();
	//2.���� �ʿ�  =  ������ - ����ġ = ����
	FVector dir = destination - me->GetActorLocation();
	//3.�������� �̵�
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
	//�ð��� �־� ���� ������
	currentTime += GetWorld()->DeltaTimeSeconds;

	//���ݽ� Ÿ���� �ٶ󺸵��� ����
	FRotator Lookat = UKismetMathLibrary::FindLookAtRotation(me->GetActorLocation(), target->GetActorLocation());
	Lookat.Pitch = 0; //���Ʒ� ��� pitch ȸ�����
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
//������ ���� ����
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
//��������

void UBossFSM::DieState()
{
	
	me->Die();


	


}
// ����� ������ ������ �Ķ����
void UBossFSM::OnDamageProcess(int _damagehp)
{
	//������ �̹��� ����ϱ�
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
			//�ǰ�
			mState = EBossState::Damage;

			//�ǰ� �ִϸ��̼�
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);

			
		}
		if (currntHp <= 3000 && !health3000)
		{
			health3000 = true;
			//�ǰ�
			mState = EBossState::Damage;

			//�ǰ� �ִϸ��̼�
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);
		}
		if (currntHp <= 2000 && !health2000)
		{
			health2000 = true;
			//�ǰ�
			mState = EBossState::Damage;

			//�ǰ� �ִϸ��̼�
			int32 index = FMath::RandRange(0, 2);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			bossAnim->PlayDamageAnim(*sectionName);
		}
		if (currntHp <= 1000 && !health1000)
		{
			health1000 = true;
			//�ǰ�
			mState = EBossState::Damage;

			//�ǰ� �ִϸ��̼�
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





