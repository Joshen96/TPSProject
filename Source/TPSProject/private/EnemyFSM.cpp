// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "EnemyAnim.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include <Runtime/AIModule/Classes/AITypes.h>
#include "Navigation/PathFollowingComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/KismetMathLibrary.h"




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
	

	currntHp = maxHp;
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
	
	//ai = Cast<AAIController>(me->GetController());


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
		

		//GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos); 
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
	
	//me->SetActorRotation()
	
	//ai->MoveToLocation(destination);
	
	
	// AI 컨트롤러 이동
	/*
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);

	ai->BuildPathfindingQuery(req, query);

	FPathFindingResult r = ns->FindPathSync(query);
	
	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(destination);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);

		if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}
	*/
	
	if (dir.Size() < attackRange)
	{

		//ai->StopMovement();

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

	//공격시 타겟을 바라보도록 설정
	FRotator Lookat = UKismetMathLibrary::FindLookAtRotation(me->GetActorLocation(), target->GetActorLocation());
	Lookat.Pitch = 0; //위아래 잠금 pitch 회전잠금
	me->SetActorRotation(Lookat);


	if (currentTime > attackDelayTime) {
		//PRINT_LOG(TEXT("Attack!!"));
		

		currentTime = 0;
		
		anim->bAttackPlay = true;
	}
	

	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

	if (distance > attackedRange )
	{
		
		mState = EEnemyState::Move;
		anim->animstate = mState;
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos);
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
		//me->GetCapsuleComponent()->SetSimulatePhysics(false);
	}
}
//죽음상태
void UEnemyFSM::DieState()
{
	//아래로떨어지도록
	//me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->SpawnExp();
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * diespeed * GetWorld()->DeltaRealTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P); 

	//me->KillCount();
	if (P.Z < -100.0f) {
		//me->KillCount();
		//me->SpawnExp();
		me->Destroy();//사라진다

	}
}
// 대미지 입을때 맞은곳 파라매터
void UEnemyFSM::OnDamageProcess(int _damagehp)
{
	//데미지 이미지 출력하기
	me->CreateDamageUI(_damagehp);

	currntHp-=_damagehp;
	//ai->StopMovement();
	FOutputDeviceNull pAR;
	me->CallFunctionByNameWithArguments(TEXT("SetupWidgets"), pAR, nullptr, true);

	//me->CreateDamageUI(_damagehp);
	//DamageWidget();
	if (currntHp > 0)
	{
		if (mState == EEnemyState::Die) 
		{
			//me->CreateDamageUI(_damagehp);
			return;
		}
		else {
			mState = EEnemyState::Damage;



			//me->GetCapsuleComponent()->SetSimulatePhysics(true);
			//FVector force = -_hitInfo.ImpactNormal * me->GetCapsuleComponent()->GetMass() * 10000; // 맞은 표면에 힘의 반대방향으로 힘을 

			//me->GetCapsuleComponent()->AddForce(force);

			//UE_LOG(LogTemp, Log, TEXT("vetor: %s"), *force.ToString());


			//피격 애니메이션
			int32 index = FMath::RandRange(0, 1);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			anim->PlayDamageAnim(*sectionName);


			//데미지 이미지 출력하기
			
			//me->CallFunctionByNameWithArguments(TEXT("CreateDamageUI"), pAR, nullptr, true);
			//me->CreateDamageUI(_damagehp);

		}
			

	}
	else
	{
		//me->KillCount();
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EEnemyState::Die;
		me->KillCount();
		
		me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		me->GetMesh()->SetSimulatePhysics(true);
		//me->kick->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
		
	}
	anim->animstate = mState;
}
//AI 제거함
bool UEnemyFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;

	return result;
}

void UEnemyFSM::DamageWidget_Implementation()
{

}

