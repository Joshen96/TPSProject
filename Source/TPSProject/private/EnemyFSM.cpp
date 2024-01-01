// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAnim.h"

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
	
	// TPS플레이어 월드에서 찾아오기
	// 찾은 플레이어를 타겟에 캐스팅
	// 이컴포넌트를 가진 액터를 가져오기
	// 
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	
	//플레이어 액터 담아주고
	target = Cast<ATPSPlayer>(actor);
	//애너미의 액터 담아주고  이제 거리계산함
	me = Cast<AEnemy>(GetOwner());

	mState = EEnemyState::Idel;
	
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
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
	//me->GetCapsuleComponent()->SetSimulatePhysics(false);
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelyTime) {
		mState = EEnemyState::Move;
		currentTime = 0;
		
		anim->animstate = mState;// move 상태로 변환
		me->GetCapsuleComponent()->SetSimulatePhysics(false);
	}
}

void UEnemyFSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("sad"));
	//1.타겟 목적지가 필요
	FVector destination = target->GetActorLocation();
	//2.방향 필요  =  목적지 - 내위치 = 방향
	FVector dir = destination - me->GetActorLocation();
	//3.방향으로 이동
	me->AddMovementInput(dir.GetSafeNormal());

	if (dir.Size() < attackRange)
	{
		mState = EEnemyState::Attck;
		anim->animstate = mState;

		anim->bAttackPlay = true;

		currentTime = attackDelayTime;
	}
}

void UEnemyFSM::AttckState()
{
	//시간을 주어 공격 딜레이
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime) {
		PRINT_LOG(TEXT("Attack!!"));


		currentTime = 0;

		anim->bAttackPlay = true;
	}


	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		anim->animstate = mState;
	}


}
//데미지 입은 상태
void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaRealTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idel;
		currentTime = 0;
		
		anim->animstate = mState;
		
	}
}
//죽음상태
void UEnemyFSM::DieState()
{
	//아래로떨어지도록
	//me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * diespeed * GetWorld()->DeltaRealTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P); 


	if (P.Z < -200.0f) {
		me->Destroy();//사라진다
	}
}
// 대미지 입을때 맞은곳 파라매터
void UEnemyFSM::OnDamageProcess(FHitResult _hitInfo)
{
	hp--;
	

	if (hp > 0)
	{
		
			mState = EEnemyState::Damage;
	
			if (me->GetCapsuleComponent()->GetCollisionEnabled() != ECollisionEnabled::NoCollision)
			{
				me->GetCapsuleComponent()->SetSimulatePhysics(true);
				FVector force = -_hitInfo.ImpactNormal * me->GetCapsuleComponent()->GetMass() * 50000; // 맞은 표면에 힘의 반대방향으로 힘을 
			
				me->GetCapsuleComponent()->AddForce(force);

				UE_LOG(LogTemp, Log, TEXT("vetor: %s"), *force.ToString());

			}

	}
	else
	{
		mState = EEnemyState::Die;
		
		
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		me->GetMesh()->SetSimulatePhysics(true);
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	anim->animstate = mState;
}

