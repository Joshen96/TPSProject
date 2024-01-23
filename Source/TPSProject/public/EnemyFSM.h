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



	UPROPERTY(EditAnywhere, Category=FSM)
	float attackedRange = 500.0f;


	//���� ������ �ð�
	float attackDelayTime = 3.0f;


	
	//�ǰݾ˸� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
	void OnDamageProcess(int _damagehp);
	//void OnDamageProcess(FHitResult _hitInfo);
	
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category=FSM)
	int32 currntHp = 0;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category=FSM)
	int32 maxHp = 10;
	
	//�ǰ� ������ �ð�
	UPROPERTY(EditAnywhere, Category=FSM)
	float damageDelayTime = 2.0f;


	// �������� �Ӽ� �Ʒ��ζ������� �ӵ�
	UPROPERTY(EditAnywhere, Category=FSM)
	float diespeed = 50.0f;

	//������� �ִϸ��̼� �������Ʈ 
	UPROPERTY()
	class UEnemyAnim* anim;

	UPROPERTY(EditAnywhere, Category = FSM)
	class AAIController* ai;

	//ai��Ʈ�� ���
	FVector randomPos;

	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void DamageWidget();
	
};
