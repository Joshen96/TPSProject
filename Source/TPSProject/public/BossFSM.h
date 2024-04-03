// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossFSM.generated.h"



UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idel,
	Move,
	Attck,
	Damage,
	Die,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	UBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EBossState mState = EBossState::Idel;

	void IdleState();

	void MoveState();

	void AttckState();

	void DamageState();

	void DieState();

	// �����ð� ��ٷȴٰ� �̵����·� ��ȯ
	// �ʿ��� �Ӽ� :  ���ð� , ����ð�

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float idleDelyTime = 2;

	float currentTime = 0;

	//��ǥ Ÿ�������� �̵�
	//�ʿ� �Ӽ� : Ÿ��

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ATPSPlayer* target;

	//��ǥ Ÿ������ �̵��ϱ����� ���� ��ġ
	//�ʿ�Ӽ� : ���� ��ġ

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ABossEnemy* me;

	//���ݹ��� 
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 150.0f;



	UPROPERTY(EditAnywhere, Category = FSM)
	float attackedRange = 500.0f;


	//���� ������ �ð�
	float attackDelayTime = 3.0f;



	//�ǰݾ˸� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
	void OnDamageProcess(int _damagehp);
	//void OnDamageProcess(FHitResult _hitInfo);

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = FSM)
	int32 currntHp = 0;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = FSM)
	int32 maxHp = 5000;

	//�ǰ� ������ �ð�
	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.0f;


	//������� �ִϸ��̼� �������Ʈ 
	UPROPERTY()
	class UEnemyAnim* anim;

	UPROPERTY(EditAnywhere, Category = FSM)
	class AAIController* ai;

	



};
