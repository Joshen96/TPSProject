// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPSPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	//발사체 물리이동 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* movementComp;

	UPROPERTY(VisibleAnywhere,Category = Collision)
	//구 콜리션 컴포넌트
	class USphereComponent* collisionComp;

	//발사체 매쉬컴포넌트
	UPROPERTY(VisibleAnywhere,Category = BodyMesh)
	class UStaticMeshComponent* bodyMeshComp;

	void Die();
	
	


	//총알 속도
	UPROPERTY(EditAnywhere, Category = Settings)
	float speed = 5000;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
