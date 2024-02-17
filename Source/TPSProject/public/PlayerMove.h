// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:

	UPlayerMove();


	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Turn(float value);

	void LookUp(float value);

	// PlayerBaseComponent 에있는것을 오버라이드 하고 입력 바인딩 재정의
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;


	//걷기 속도
	//UPROPERTY(EditAnywhere, Category = PlayerSetting)
	//float walkSpeed = 200;
	//뛰기 속도
	//UPROPERTY(EditAnywhere, Category = PlayerSetting)
	//float runSpeed = 500;

	//이동관련함수
	void InputHorizontal(float value);

	void InputVertical(float value);

	void InputJump();

	void Move();

	void InputRun();

	void InputWalk();

	FVector dir;




	
};
