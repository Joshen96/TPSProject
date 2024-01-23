// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idel,
	Move,
	Attck,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
public:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=FSM)
	EEnemyState mState = EEnemyState::Idel;

	void IdleState();

	void MoveState();

	void AttckState();

	void DamageState();

	void DieState();

	// 일정시간 기다렸다가 이동상태로 전환
	// 필요한 속성 :  대기시간 , 경과시간
	
	UPROPERTY(EditDefaultsOnly,Category=FSM)
	float idleDelyTime = 2;

	float currentTime = 0;

	//목표 타깃쪽으로 이동
	//필요 속성 : 타겟

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* target;

	//목표 타깃으로 이동하기위해 나의 위치
	//필요속성 : 나의 위치

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AEnemy* me;
	
	//공격범위 
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;



	UPROPERTY(EditAnywhere, Category=FSM)
	float attackedRange = 500.0f;


	//공격 딜레이 시간
	float attackDelayTime = 3.0f;


	
	//피격알림 이벤트 함수
	UFUNCTION(BlueprintCallable)
	void OnDamageProcess(int _damagehp);
	//void OnDamageProcess(FHitResult _hitInfo);
	
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category=FSM)
	int32 currntHp = 0;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category=FSM)
	int32 maxHp = 10;
	
	//피격 딜레이 시간
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;


	// 죽음상태 속성 아래로떨어지는 속도
	UPROPERTY(EditAnywhere, Category=FSM)
	float diespeed = 50.0f;

	//사용중인 애니메이션 블루프린트 
	UPROPERTY()
	class UEnemyAnim* anim;

	UPROPERTY(EditAnywhere, Category = FSM)
	class AAIController* ai;

	//ai패트롤 기능
	FVector randomPos;

	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void DamageWidget();
	
};
