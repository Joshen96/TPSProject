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

	//암컴포넌트
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	//카메라 컴포넌트

	UPROPERTY(VisibleAnywhere,BluePrintReadOnly, Category=Camera)
	class UCameraComponent* tpsCamComp;

	//총매쉬 

	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	//스나이퍼 총 매쉬
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* sniperMeshComp;

	

	//총발사 구현부분

	//1. 발사구현 팩토리 구현
	UPROPERTY(EditDefaultsOnly,Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	//발사 사운드 
	UPROPERTY(EditDefaultsOnly,Category=Sound)// 아키타입에서 편집가능
	class USoundBase* bulletsound;


	//2. 발사함수
	void InputFire();

	//무브 컴포넌트 부착

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;

	//3. 이동 부분


	

	//4. 총교체 함수

	bool bUseingGrenadeGun = false;

	void ChangeGrenadeGun();

	void ChangeSniperGun();

	//5. 스나이퍼 줌 모드

	void SniperZoom();
	
	

	bool bSniperAim = true;


	//스나이퍼 줌 UI관련 

	UPROPERTY(EditDefaultsOnly, Category=SniperUI) //외부에서 UI 가져오고
	TSubclassOf<class UUserWidget> sniperUIFactory;
	//여기서 사용할 스나이퍼 유아이 인스턴스
	class UUserWidget* _sniperUI;
	//스나이퍼 총알 파티클

	//크로스헤어 ui 관련
	UPROPERTY(EditDefaultsOnly, Category=SniperUI) //외부에서 UI 가져오고
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	//여기서 사용할 스나이퍼 유아이 인스턴스
	class UUserWidget* _crosshairUI;

	UPROPERTY(EditDefaultsOnly, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;


	// 카메라쉐이크 

	UPROPERTY(EditDefaultsOnly,Category=CameraMotion)
	TSubclassOf<class UCameraShakeBase> cameraShake;

};
