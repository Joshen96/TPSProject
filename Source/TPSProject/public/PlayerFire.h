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

	//총매쉬 
	UPROPERTY()
	class USkeletalMeshComponent* gunMeshCompRight;

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshCompLeft;

	//스나이퍼 총 매쉬
	UPROPERTY()
	class USkeletalMeshComponent* sniperMeshComp;



	//발사체 팩토리 구현
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class AActor> bulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = ItemHitcheck)
	TSubclassOf<class AActor> ItemcheckActor;

	UPROPERTY(VisibleAnywhere, Category = Player)
	class ATPSPlayer* mes;

	//함수 부분
	
	UFUNCTION(BlueprintCallable)
	void InputFire();


	void SniperZoom();

	void DelayGrenadeGun();


	//bool 부분
	

	bool bSniperAim = true;

	UPROPERTY(Editanywhere, BlueprintReadWrite, category = Damage)
	float SubGunShotDelay = 1.0f;



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnShotBullet();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = LVUP)
	void BasicGunDamageUP();


	//스나이퍼 총알 파티클
	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectDefault;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectBlood;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectGround;

	

	//발사 사운드 
	UPROPERTY(EditDefaultsOnly, Category = Sound)// 아키타입에서 편집가능
	class USoundBase* bulletsound;

	// 카메라쉐이크 

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

};
