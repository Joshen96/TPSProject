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

	//¾ÏÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, Category=Camera)
	class USpringArmComponent* springArmComp;
	//Ä«¸Þ¶ó ÄÄÆ÷³ÍÆ®

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* tpsCamComp;

	//ÃÑ¸Å½¬ 

	UPROPERTY(VisibleAnywhere, Category=GunMesh)
	class USkeletalMeshComponent* gunMeshComp;

	float moveSpeed = 500;

	void Turn(float value);

	void LookUp(float value);

	void InputHorizontal(float value);

	void InputVertical(float value);

	void InputJump();

	void Move();
	
	FVector dir;
};
