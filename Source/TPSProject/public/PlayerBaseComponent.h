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
		PrimaryComponentTick.bCanEverTick = false; // 틱비활성화
		bWantsInitializeComponent = true; // BeginPlay보다 먼저 실행되는 초기화컴포넌트 호출되도록 설정
	};

	virtual void InitializeComponent() override;


	// Called when the game starts
	virtual void BeginPlay() override;

public:	


	//사용자 입력 매핑 처리함수
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) {};


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//이컴포넌트를 소유한 액터
	UPROPERTY()
	ATPSPlayer* me;


	//캐릭터 움직임컴포턴트
	UCharacterMovementComponent* moveComp;
};
