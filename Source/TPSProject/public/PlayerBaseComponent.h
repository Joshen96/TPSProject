// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBaseComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UPlayerBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerBaseComponent() {
		PrimaryComponentTick.bCanEverTick = false; // ƽ��Ȱ��ȭ
	};


	// Called when the game starts
	virtual void BeginPlay() override;

public:	


	//����� �Է� ���� ó���Լ�
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) {};


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//��������Ʈ�� ������ ����
	UPROPERTY()
	ATPSPlayer* me;


	//ĳ���� ������������Ʈ
	UCharacterMovementComponent* moveComp;
};
