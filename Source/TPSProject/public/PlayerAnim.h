// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	

	//블루 프린트의 Event Blueprint Update Animation노드와 대응되는 함수
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//사용할 변수 들
	//속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim) //애니블부프린트에서 가져와 사용가능하도록 BlueprintReadWrite 와 블루프린트에서 편집가능하도록 EditDefaultsOnly 
	float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim) //애니블부프린트에서 가져와 사용가능하도록 BlueprintReadWrite 와 블루프린트에서 편집가능하도록 EditDefaultsOnly 
	float direction = 0;

	// 공중인지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim) //애니블부프린트에서 가져와 사용가능하도록 BlueprintReadWrite 와 블루프린트에서 편집가능하도록 EditDefaultsOnly 
	bool isInAir = false;


	//몽타주 만든것 넣어줘야함
	UPROPERTY(EditDefaultsOnly, Category=Player)
	class UAnimMontage* attckAnimMontage;

	void PlayAttckAnim();


};
