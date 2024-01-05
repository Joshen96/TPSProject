// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyFSM.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <PlayerAnim.h>
#include "PlayerMove.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�ʱ⼼�úκ�
	// ���� ������Ʈ
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	
	 
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
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);// ���忡�� �������� UI ������ UI �ν���Ʈ�� ��������

	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);


	ChangeSniperGun();// ���� �������۷ν���
	bSniperAim = false;
	_crosshairUI->AddToViewport();


	


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






	//�Է� �����ϱ�
	//ī�޶� ������ ������Ʈ�� ����� ���� �����ϵ��� ����
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	//PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	//PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	////���ε� �׼��� IE_Pressed ó�� ������������ ���������
	//PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &ATPSPlayer::InputJump);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeGrenadeGun);
	
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeSniperGun);

	
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperZoom);
	
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperZoomOut); //���׷� ����

	


}

void ATPSPlayer::InputFire()
{
	//�ִϸ��̼� ����
	 auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	 anim->PlayAttckAnim();// �޾ƿͼ� ��ɽ���
	
	 //ī�޶� ����ũ ����

	 auto controller = GetWorld()->GetFirstPlayerController();
	 //���� ī�޶� ����ũ �������Ʈ�� �־ ����
	 controller->PlayerCameraManager->StartCameraShake(cameraShake);


	 //���� ����
	 UGameplayStatics::PlaySound2D(GetWorld(), bulletsound);

	//��Ÿ�Ե��� ����
	if (bUseingGrenadeGun) {
		FTransform firePos = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePos);

	}
	else
	{
		//�������� �ѹ߻� ����Ʈ���̼� ���
		// ī�޶���ۺ���
		FVector startPos = tpsCamComp->GetComponentLocation();
		// ī�޶��� �ձ���
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;

		FHitResult hitInfo; // �����޾ƿ���

		FCollisionQueryParams params; // �浹�ɼ�?

		params.AddIgnoredActor(this); // �ڱ��ڽ��� �����ϵ��� ����
		
		// ������ �ϳ����� �Ѱ��� �������� (������, ���۰� ��, ä��, �浹�ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// �¾Ҵ��� ����
		if (bHit) {

			FTransform bulletTrans; //��ġ ������� Ʈ������

			bulletTrans.SetLocation(hitInfo.ImpactPoint); //Ʈ�������� ���������� ����

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory,bulletTrans); //�װ��� ����Ʈ ����

			auto hitComp = hitInfo.GetComponent(); 
			// �������� ������Ʈ ��������

			if (hitComp && hitComp->IsSimulatingPhysics()) //�������� �ְ� �������� �������밡���ϴٸ� 
			{
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 50000; // ���� ǥ�鿡 ���� �ݴ�������� ���� 
			
				hitComp->AddForce(force);
			}

			//�ε��� ����� ���� ������ �Ǵ��ϱ�

			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy) {
				auto enemyFSM = Cast<UEnemyFSM>(enemy);

				enemyFSM->OnDamageProcess(hitInfo);


			}
		}
	
	}
}





void ATPSPlayer::ChangeGrenadeGun()
{
	if (bSniperAim) {
		return;
	}
	bUseingGrenadeGun = true;
	sniperMeshComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeSniperGun()
{

	bUseingGrenadeGun = false;
	sniperMeshComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

//void ATPSPlayer::SniperZoomOut()
//{
//	if (bUseingGrenadeGun) {
//		return;
//
//	}
//	if (bSniperAim) {
//		bSniperAim = false;
//		_sniperUI->RemoveFromParent(); // UI�� ����Ʈ�� ���̴°��� ����
//		tpsCamComp->SetFieldOfView(90.0f); // ī�޶� �並 90���� ���� �ܾƿ�
//		_crosshairUI->AddToViewport();
//		
//	}
//}

void ATPSPlayer::SniperZoom()
{
	if (bUseingGrenadeGun) {
		return;

	}
	if (!bSniperAim) {
		bSniperAim = true;
		_sniperUI->AddToViewport();		// ��������UI�� ����Ʈ�� ���
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(45.0f); // ī�޶� �並 45.0�� ���� ����
		
		_crosshairUI->RemoveFromParent();
		
	}
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent(); // UI�� ����Ʈ�� ���̴°��� ����
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(90.0f); // ī�޶� �並 90���� ���� �ܾƿ�
		
		_crosshairUI->AddToViewport();
	}

	
}
