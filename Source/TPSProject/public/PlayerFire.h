// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent),meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	

public:

	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY()
	class UCameraComponent* tpsCamComp;

	//�ѸŽ� 

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;

	//�������� �� �Ž�
	UPROPERTY()
	class USkeletalMeshComponent* sniperMeshComp;






	//�߻�ü ���丮 ����
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;



	//�Լ� �κ�
	void InputFire();

	void ChangeGrenadeGun();

	void ChangeSniperGun();

	void SniperZoom();

	//bool �κ�
	bool bUseingGrenadeGun = false;

	bool bSniperAim = true;



	//�������� �� UI���� 
	UPROPERTY(EditDefaultsOnly, Category = SniperUI) //�ܺο��� UI ��������
	TSubclassOf<class UUserWidget> sniperUIFactory;

	//�⺻ ũ�ν���� ui ����
	UPROPERTY(EditDefaultsOnly, Category = SniperUI) //�ܺο��� UI ��������
	TSubclassOf<class UUserWidget> crosshairUIFactory;



	//���⼭ ����� �������� ������ �ν��Ͻ�
	class UUserWidget* _sniperUI;
	

	//���⼭ ����� ũ�ν���� ������ �ν��Ͻ�
	class UUserWidget* _crosshairUI;




	//�������� �Ѿ� ��ƼŬ
	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;
	//�߻� ���� 
	UPROPERTY(EditDefaultsOnly, Category = Sound)// ��ŰŸ�Կ��� ��������
	class USoundBase* bulletsound;

	// ī�޶���ũ 

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

};
