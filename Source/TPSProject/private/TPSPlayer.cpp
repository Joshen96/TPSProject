// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�ʱ⼼�úκ�
	//���̷��� �ҷ�����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);// �ҷ��� �Ž����� ����

		//�Ž���ġ �����ϱ�
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, 90), FRotator(0, 0, -90));
	}


	// ī�޶�� ������ ������ ī�޶�ϼ���
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent); //���� ��Ʈ�� ������
	springArmComp->SetRelativeLocation(FVector(0, 70, 90)); // ȸ�� ��ġ����
	springArmComp->TargetArmLength = 400; // Ÿ�ٰŸ� ����
	//�������� yawȸ�� ���
	springArmComp->bUsePawnControlRotation = true;

	//ī�޶� �����ϱ�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);//ķ�� �Ͽ� �����Ѵ�
	//ī�޶��� yawȸ���� ���� ��������ȸ������ ����ϱ�����
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	// �ֻ����� �÷��̾��� Yawȸ�� ����ϱ�����
	

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
	//�Է� �����ϱ�

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value); //��ȸ������ �Է°� ����
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value); //��ġ������ �Է°� ����
}

