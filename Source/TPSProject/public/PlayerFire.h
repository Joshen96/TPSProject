// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom),meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	

public:

	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(Editanywhere, BlueprintReadWrite, category=Damage)
	float sniper_Damage =10;


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

	UPROPERTY(EditDefaultsOnly, Category = ItemHitcheck)
	TSubclassOf<class AActor> ItemcheckActor;

	UPROPERTY(VisibleAnywhere, Category = Player)
	class ATPSPlayer* mes;

	//�Լ� �κ�
	
	UFUNCTION(BlueprintCallable)
	void InputFire();


	void SniperZoom();

	void DelayGrenadeGun();


	//bool �κ�
	

	bool bSniperAim = true;

	UPROPERTY(Editanywhere, BlueprintReadWrite, category = Damage)
	float SubGunShotDelay = 1.0f;



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
