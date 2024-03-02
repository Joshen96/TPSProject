// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h" //헤더추가
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	//매쉬가져오고
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

		GetMesh()->SetRelativeScale3D(FVector(0.84f));

	}
	

	kick = CreateDefaultSubobject<USphereComponent>(TEXT("kickcollision"));

	kick->SetupAttachment(GetMesh(), TEXT("kick_Pos"));
	
	
	
	kick->SetCollisionProfileName(TEXT("EnemyKick"));

	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
	//애너미 애님블루프린트 할당하기

	//ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/'Engine.AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));

	//if (tempClass.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(tempClass.Class);
	//}

	//애니메이션 가져오기


	//AI컨트롤러 소유 자동할당

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void AEnemy::CreateDamageUI(int _Damage)
//{

//}



void AEnemy::kickDeActive()
{
	kick->SetSphereRadius(0.1);
}





