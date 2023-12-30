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

	// �����ð� ��ٷȴٰ� �̵����·� ��ȯ
	// �ʿ��� �Ӽ� :  ���ð� , ����ð�
	
	UPROPERTY(EditDefaultsOnly,Category=FSM)
	float idleDelyTime = 2;

	float currentTime = 0;

	//��ǥ Ÿ�������� �̵�
	//�ʿ� �Ӽ� : Ÿ��

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* target;

	//��ǥ Ÿ������ �̵��ϱ����� ���� ��ġ
	//�ʿ�Ӽ� : ���� ��ġ

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class AEnemy* me;
	
	//���ݹ��� 
	UPROPERTY(EditAnywhere, Category=FSM)
	float attackRange = 150.0f;

	//���� ������ �ð�
	float attackDelayTime = 2.0f;


	//�ǰݾ˸� �̺�Ʈ �Լ�
	void OnDamageProcess(FHitResult _hitInfo);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
	int32 hp = 3;

	//�ǰ� ������ �ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;


	// �������� �Ӽ� �Ʒ��ζ������� �ӵ�
	UPROPERTY(EditAnywhere, Category=FSM)
	float diespeed = 50.0f;


};