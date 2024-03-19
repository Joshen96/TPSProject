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

	// PlayerBaseComponent ���ִ°��� �������̵� �ϰ� �Է� ���ε� ������
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;


	//�̵������Լ�
	void InputHorizontal(float value);

	void InputVertical(float value);

	void InputJump();

	void Move();

	void InputRun();

	void InputWalk();

	FVector dir;




	
};
