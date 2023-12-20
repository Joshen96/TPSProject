// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//초기세팅부분
	//스켈레톤 불러오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);// 불러온 매쉬장착 해줌

		//매시위치 조정하기
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, 90), FRotator(0, 0, -90));
	}


	// 카메라암 붙히기 선언한 카메라암세팅
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent); //상위 루트에 붙히고
	springArmComp->SetRelativeLocation(FVector(0, 70, 90)); // 회전 위치조정
	springArmComp->TargetArmLength = 400; // 타겟거리 조절
	//스프링암 yaw회전 사용
	springArmComp->bUsePawnControlRotation = true;

	//카메라 세팅하기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);//캠을 암에 부착한다
	//카메라의 yaw회전은 끄기 스프링암회전으로 사용하기위함
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	// 최상위인 플레이어의 Yaw회전 사용하기위함
	

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//입력 매핑하기

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value); //요회전으로 입력값 적용
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value); //피치값으로 입력값 적용
}

