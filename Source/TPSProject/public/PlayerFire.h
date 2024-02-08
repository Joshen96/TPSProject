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


	UPROPERTY(Editanywhere,BlueprintReadWrite, category=Damage)
	int sniper_Damage =10;



	UPROPERTY()
	class UCameraComponent* tpsCamComp;

	//�ѸŽ� 

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshCompRight;

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshCompLeft;

	//�������� �� �Ž�
	UPROPERTY()
	class USkeletalMeshComponent* sniperMeshComp;






	//�߻�ü ���丮 ����
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class AActor> bulletFactory;



	//�Լ� �κ�
	
	UFUNCTION(BlueprintCallable)
	void InputFire();

	void UseGrenadeGun();

	void ChangeSniperGun();

	void SniperZoom();

	void DelayGrenadeGun();


	//bool �κ�
	
	bool bUseingGrenadeGun = false;

	bool bSniperAim = true;

	UPROPERTY(Editanywhere, category = Damage)
	float GrenadeShotDelay = 1.0f;


	//�������� �� UI���� 
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI) //�ܺο��� UI ��������
	//class UUserWidget* sniperUIFactory;

	//�⺻ ũ�ν���� ui ����
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI) //�ܺο��� UI ��������
	//		class UUserWidget* crosshairUIFactory;



	//���⼭ ����� �������� ������ �ν��Ͻ�
	//class UUserWidget* _sniperUI;
	

	//���⼭ ����� ũ�ν���� ������ �ν��Ͻ�
	//class UUserWidget* _crosshairUI;

	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnShotBullet();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = LVUP)
	void BasicGunDamageUP();

	

	//�������� �Ѿ� ��ƼŬ
	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectDefault;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectBlood;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectGround;



	//�߻� ���� 
	UPROPERTY(EditDefaultsOnly, Category = Sound)// ��ŰŸ�Կ��� ��������
	class USoundBase* bulletsound;

	// ī�޶���ũ 

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

};
