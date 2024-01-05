// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMove.h" //�̵�������Ʈ
#include "PlayerFire.h" //���� ������Ʈ


// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�ʱ⼼�úκ�
	
	// ���� ������Ʈ
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// ����� ���� ������Ʈ �Ҵ�

	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));

	
	 
	//���̷��� �ҷ�����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);// �ҷ��� �Ž����� ����

		//�Ž���ġ �����ϱ�
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
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
	

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSoket")); //���� ��ü�� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	//�������� �Ž��������
	sniperMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunMeshComp"));

	sniperMeshComp->SetupAttachment(GetMesh(),TEXT("hand_rSoket"));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/sniper1.sniper1'"));

	if (TempSniperMesh.Succeeded())
	{
		sniperMeshComp->SetSkeletalMesh(TempSniperMesh.Object);// �Ž� �Ҵ��ϱ�

		sniperMeshComp->SetRelativeLocation(FVector(-22, 55, 120));//��ġ ����

		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));//�Ž� ũ������
	}

	
	
}


// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//�ʱ� �ӵ� �ȱ�� ���� //������Ʈ �̵����� ����
	//GetCharacterMovement()->MaxWalkSpeed = walkSpeed;


	//�������� ��������


	


}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ������Ʈ�� �̵�
	//Move();

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//������Ʈ ���� �Է� ���ε� ó���ϵ��� �ϱ�
	playerMove->SetupInputBinding(PlayerInputComponent);
	playerFire->SetupInputBinding(PlayerInputComponent);




}
