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



	//함수 부분
	
	UFUNCTION(BlueprintCallable)
	void InputFire();

	void UseGrenadeGun();

	void ChangeSniperGun();

	void SniperZoom();

	void DelayGrenadeGun();


	//bool 부분
	
	bool bUseingGrenadeGun = false;

	bool bSniperAim = true;

	UPROPERTY(Editanywhere, category = Damage)
	float GrenadeShotDelay = 1.0f;


	//스나이퍼 줌 UI관련 
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI) //외부에서 UI 가져오고
	//class UUserWidget* sniperUIFactory;

	//기본 크로스헤어 ui 관련
	//UPROPERTY(EditDefaultsOnly, Category = SniperUI) //외부에서 UI 가져오고
	//		class UUserWidget* crosshairUIFactory;



	//여기서 사용할 스나이퍼 유아이 인스턴스
	//class UUserWidget* _sniperUI;
	

	//여기서 사용할 크로스헤어 유아이 인스턴스
	//class UUserWidget* _crosshairUI;

	

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
