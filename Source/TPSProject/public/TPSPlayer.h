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

	

	//움직임속도용
	float moveSpeed = 500;

	//총발사 구현부분

	//1. 발사구현 팩토리 구현
	UPROPERTY(EditDefaultsOnly,Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	

	//2. 발사함수
	void InputFire();

	//3. 이동 부분
	void Turn(float value);

	void LookUp(float value);

	void InputHorizontal(float value);

	void InputVertical(float value);

	void InputJump();

	void Move();
	
	FVector dir;

	//4. 총교체 함수

	bool bUseingGrenadeGun = false;

	void ChangeGrenadeGun();

	void ChangeSniperGun();

	//5. 스나이퍼 줌 모드

	void SniperAim();

	bool bSniperAim = true;


	//스나이퍼 줌 UI관련 

	UPROPERTY(EditDefaultsOnly, Category=SniperUI) //외부에서 UI 가져오고
	TSubclassOf<class UUserWidget> sniperUIFactory;
	//여기서 사용할 스나이퍼 유아이 인스턴스
	class UUserWidget* _sniperUI;

};
