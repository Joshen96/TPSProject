// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	
	// TPS�÷��̾� ���忡�� ã�ƿ���
	// ã�� �÷��̾ Ÿ�ٿ� ĳ����
	// ��������Ʈ�� ���� ���͸� ��������
	// 
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	
	//�÷��̾� ���� ����ְ�
	target = Cast<ATPSPlayer>(actor);
	//�ֳʹ��� ���� ����ְ�  ���� �Ÿ������
	me = Cast<AEnemy>(GetOwner());

	mState = EEnemyState::Idel;
	

	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

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

void UEnemyFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelyTime) {
		mState = EEnemyState::Move;
		currentTime = 0;
		
		
		me->GetCapsuleComponent()->SetSimulatePhysics(false);
	}
}

void UEnemyFSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("sad"));
	//1.Ÿ�� �������� �ʿ�
	FVector destination = target->GetActorLocation();
	//2.���� �ʿ�  =  ������ - ����ġ = ����
	FVector dir = destination - me->GetActorLocation();
	//3.�������� �̵�
	me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attck;
	}
}

void UEnemyFSM::AttckState()
{
	//�ð��� �־� ���� ������
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime) {
		PRINT_LOG(TEXT("Attack!!"));


		currentTime = 0;
	}


	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
	}


}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idel;
		currentTime = 0;
	}
}
//��������
void UEnemyFSM::DieState()
{
	//�Ʒ��ζ���������
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * diespeed * GetWorld()->DeltaRealTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);


	if (P.Z < -200.0f) {
		me->Destroy();//�������
	}
}

void UEnemyFSM::OnDamageProcess(FHitResult _hitInfo)
{
	hp--;
	

	if (hp > 0)
	{
		mState = EEnemyState::Damage;

		me->GetCapsuleComponent()->SetSimulatePhysics(true);
		
		FVector force = -_hitInfo.ImpactNormal * me->GetCapsuleComponent()->GetMass() * 200000; // ���� ǥ�鿡 ���� �ݴ�������� ���� 

		me->GetCapsuleComponent()->AddForce(force);
	}
	else
	{
		mState = EEnemyState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

