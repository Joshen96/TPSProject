// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//��������Ʈ
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	//ī�޶� ������Ʈ

	UPROPERTY(VisibleAnywhere,BluePrintReadOnly, Category=Camera)
	class UCameraComponent* tpsCamComp;

	//�ѸŽ� 

	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//�������� �� �Ž�
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* sniperMeshComp;

	

	//�����Ӽӵ���
	float moveSpeed = 500;

	//�ѹ߻� �����κ�

	//1. �߻籸�� ���丮 ����
	UPROPERTY(EditDefaultsOnly,Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	

	//2. �߻��Լ�
	void InputFire();

	//3. �̵� �κ�
	void Turn(float value);

	void LookUp(float value);

	void InputHorizontal(float value);

	void InputVertical(float value);

	void InputJump();

	void Move();
	
	FVector dir;

	//4. �ѱ�ü �Լ�

	bool bUseingGrenadeGun = false;

	void ChangeGrenadeGun();

	void ChangeSniperGun();

	//5. �������� �� ���

	void SniperAim();

	bool bSniperAim = true;


	//�������� �� UI���� 

	UPROPERTY(EditDefaultsOnly, Category=SniperUI) //�ܺο��� UI ��������
	TSubclassOf<class UUserWidget> sniperUIFactory;
	//���⼭ ����� �������� ������ �ν��Ͻ�
	class UUserWidget* _sniperUI;

};
