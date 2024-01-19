// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledObjects.h"
#include "Bullet_OP.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API ABullet_OP : public APooledObjects
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABullet_OP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	//�߻�ü �����̵� ������Ʈ
	//UPROPERTY(VisibleAnywhere, Category = Movement)
	//class UProjectileMovementComponent* movementComp;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	//�� �ݸ��� ������Ʈ
	class USphereComponent* collisionComp;

	//�߻�ü �Ž�������Ʈ
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;

	void Die();


	
	UPROPERTY(EditAnywhere, Category = Settings)
	float LifeTime = 0;
	//�Ѿ� �ӵ�
	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;

	UPROPERTY(EditAnywhere, Category = Settings)
	int bulletDamage = 0;
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


	//�浹�ݸ��� ����

	UFUNCTION()
	void OnEnemyTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�Ѿ� �߻�ü ���ְ� �����̵�
	UFUNCTION()
	void FrontShot(float DeltaTime);



	

	//void SimulationEnabled(bool type);
	
	//void Deactivate() override;
	
};
