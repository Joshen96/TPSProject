// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossEnemy.generated.h"


UCLASS()
class TPSPROJECT_API ABossEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = FSMComponent)
	class UBossFSM* bossfsm1; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM)
	class ATPSPlayer* target;

	UFUNCTION(BlueprintImplementableEvent, Category = Hit)
	void Hit();

	UFUNCTION(BlueprintImplementableEvent, Category = AttacK)
	void Attackstart();

	UFUNCTION(BlueprintImplementableEvent, Category = AttacK)
	void Attackend();

	void DestoryEnemy();

	UFUNCTION(BlueprintCallable)
	void LaunchForward(float LaunchSpeed);

	UFUNCTION(BlueprintCallable)
	void LookatTarget();

	UFUNCTION(BlueprintImplementableEvent, Category = CreateUI)
	void CreateDamageUI(int _Damage);
	
};
