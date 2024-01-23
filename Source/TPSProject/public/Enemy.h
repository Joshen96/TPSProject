// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UCLASS()
class TPSPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category=FSMComponent)
	class UEnemyFSM* fsm; //컴포넌트 단독사용 불가로 에너미에 부착하여 사용하기위함


	
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = kick)
	class USphereComponent* kick;


	UFUNCTION(BlueprintImplementableEvent,Category = CreateUI)
	void CreateDamageUI(int _Damage);



	//UFUNCTION()
	//void OnEnemyTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void kickDeActive();

	UFUNCTION(BlueprintImplementableEvent, Category = KillCount)
	void KillCount();
};
