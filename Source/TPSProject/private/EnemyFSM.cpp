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
		
		anim->animstate = mState;// move ���·� ��ȯ
		

		//GetRandomPositionInNavMesh(me->GetActorLocation(), 500, randomPos); 
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
	
	//me->SetActorRotation()
	
	//ai->MoveToLocation(destination);
	
	
	// AI ��Ʈ�ѷ� �̵�
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
	//�ð��� �־� ���� ������
	currentTime += GetWorld()->DeltaTimeSeconds;

	//���ݽ� Ÿ���� �ٶ󺸵��� ����
	FRotator Lookat = UKismetMathLibrary::FindLookAtRotation(me->GetActorLocation(), target->GetActorLocation());
	Lookat.Pitch = 0; //���Ʒ� ��� pitch ȸ�����
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
//������ ���� ����
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
//��������
void UEnemyFSM::DieState()
{
	//�Ʒ��ζ���������
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
		me->Destroy();//�������

	}
}
// ����� ������ ������ �Ķ����
void UEnemyFSM::OnDamageProcess(int _damagehp)
{
	//������ �̹��� ����ϱ�
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
			//FVector force = -_hitInfo.ImpactNormal * me->GetCapsuleComponent()->GetMass() * 10000; // ���� ǥ�鿡 ���� �ݴ�������� ���� 

			//me->GetCapsuleComponent()->AddForce(force);

			//UE_LOG(LogTemp, Log, TEXT("vetor: %s"), *force.ToString());


			//�ǰ� �ִϸ��̼�
			int32 index = FMath::RandRange(0, 1);
			FString sectionName = FString::Printf(TEXT("Damage%d"), index);
			anim->PlayDamageAnim(*sectionName);


			//������ �̹��� ����ϱ�
			
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
//AI ������
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

