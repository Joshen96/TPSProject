// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"

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
	gunMeshComp->SetupAttachment(GetMesh()); //���� ��ü�� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	//�������� �Ž��������
	sniperMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunMeshComp"));

	sniperMeshComp->SetupAttachment(GetMesh());


	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SniperGun/sniper1.sniper1'"));

	if (TempGunMesh.Succeeded())
	{
		sniperMeshComp->SetSkeletalMesh(TempGunMesh.Object);// �Ž� �Ҵ��ϱ�

		sniperMeshComp->SetRelativeLocation(FVector(-22, 55, 120));//��ġ ����

		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));//�Ž� ũ������
	}

}


// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

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


	Move();

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//�Է� �����ϱ�

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	//���ε� �׼��� IE_Pressed ó�� ������������ ���������
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &ATPSPlayer::InputJump);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeGrenadeGun);
	
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeSniperGun);

	
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);


}

void ATPSPlayer::InputFire()
{

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
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 500000; // ���� ǥ�鿡 ���� �ݴ�������� ���� 

				hitComp->AddForce(force);
			}
		}
	
	}
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value); //��ȸ������ �Է°� ����
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value); //��ġ������ �Է°� ����
}

void ATPSPlayer::InputHorizontal(float value)
{
	dir.Y = value;
}

void ATPSPlayer::InputVertical(float value)
{
	dir.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::Move()
{
	dir = FTransform(GetControlRotation()).TransformVector(dir); //������ǥ�� �����ǥ �������� �ٲ���
	// ���� �̵�
	// ������ġ + ���� * �ӵ� * �ð� =  �����ġ
	/*FVector P;

	FVector NowL = GetActorLocation();

	FVector vt = dir * moveSpeed * DeltaTime;

	P = vt + NowL;

	SetActorLocation(P);*/
	// ��ü�ϱ�
	AddMovementInput(dir);

	dir = FVector::ZeroVector;
}

void ATPSPlayer::ChangeGrenadeGun()
{
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

void ATPSPlayer::SniperAim()
{
	if (bUseingGrenadeGun) {
		return;

	}
	if (bSniperAim == false)//�ܸ��
	{
		bSniperAim = true; //�������� ���� ���� ����
		_sniperUI->AddToViewport();		// ��������UI�� ����Ʈ�� ��¤�
		tpsCamComp->SetFieldOfView(45.0f); // ī�޶� �並 45.0�� ���� ����
		_crosshairUI->RemoveFromParent();

	}
	else// ������
	{
		bSniperAim = false; // �������ۿ��� ���� ����
		_sniperUI->RemoveFromParent(); // UI�� ����Ʈ�� ���̴°��� ����
		tpsCamComp->SetFieldOfView(90.0f); // ī�޶� �並 90���� ���� �ܾƿ�
		_crosshairUI->AddToViewport();

	}
}