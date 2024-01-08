// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate,class UInputComponent*); // ��Ƽĳ��Ʈ�� �Է�������Ʈ�� �޴� ��������Ʈ



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




	
	//���� ������Ʈ ����

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerMove;

	//���� ������Ʈ ����

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* playerFire;


	//ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 hp;

	//�ʱ� ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 initiaHp = 10;


	UFUNCTION(BlueprintCallable, Category = Health)
	//�ǰ� �̺�Ʈ
	void onHitEvent();

	//�ǰ� �ݸ���
	UFUNCTION()
	void OnEnemyKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);




	

};
