// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate,class UInputComponent*); // 멀티캐스트용 입력컴포넌트를 받는 델리게이트



//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyDelegate1, FName, name)

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();
	//FMyDelegate1 myVar;
	//FMyDelegate2 myMVar;
	FInputBindingDelegate onInputBindingDelegate;


	//void TestFunc(FName name);
	//void TestFunc2(FName name);
	//void PlayDelegate();
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshCompRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshCompLeft;



	//스나이퍼 총 매쉬
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* sniperMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bUseingGrenadeGun = false;

	
	//무브 컴포넌트 부착

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;

	//공격 컴포넌트 부착

	UPROPERTY(Editanywhere, BlueprintReadWrite,  Category = Component)
	class UPlayerBaseComponent* playerFire;


	//체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 hp=0;

	//초기 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 initiaHp = 10;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSpeed)
	float walkSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSpeed)
	float runSpeed = 800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerSpeed)
	bool isRunning = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerState)
	bool isHit = false;

	int HitIndex =0;

	UPROPERTY(EditAnywhere, Category = PlayerState)
	float damageDelayTime = 1.5f;

	float currentTime = 0;
	//공격 부분
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = fireSet)
	float BasicGunDamage = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = fireSet)
	float SubGunDelay=0;	

	UFUNCTION(BlueprintCallable, Category = Health)
	//피격 이벤트
	void onHitEvent();


	UFUNCTION(BlueprintCallable, Category = Health)
	//피격 무적기능시간
	void PlayerhitTimeCheck();

	UFUNCTION(BlueprintCallable, Category = Health)
	void PlayerMeshBlink();

	//피격 콜리젼
	UFUNCTION()
	void OnEnemyKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//오브젝트 풀링 탑재
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = OBJPool)
	class UObjectPools* ObjectPool;

	//게임오버 함수

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnGameOver();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
	void OnUsingGrenade(bool isGrenade);


	UFUNCTION(BlueprintCallable)
	void UseGrenadeGun();

	
	
};
