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
	//발사체 물리이동 컴포넌트
	//UPROPERTY(VisibleAnywhere, Category = Movement)
	//class UProjectileMovementComponent* movementComp;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	//구 콜리션 컴포넌트
	class USphereComponent* collisionComp;

	//발사체 매쉬컴포넌트
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;

	void Die();


	
	UPROPERTY(EditAnywhere, Category = Settings)
	float LifeTime = 0;
	//총알 속도
	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;

	UPROPERTY(EditAnywhere, Category = Settings)
	int bulletDamage = 0;
	//virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


	//충돌콜리젼 구현

	UFUNCTION()
	void OnEnemyTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//총알 발사체 없애고 전방이동
	UFUNCTION()
	void FrontShot(float DeltaTime);



	

	//void SimulationEnabled(bool type);
	
	//void Deactivate() override;
	
};
