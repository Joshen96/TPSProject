// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMove.h" //�̵�������Ʈ
#include "PlayerFire.h" //���� ������Ʈ
#include "Components/CapsuleComponent.h"
#include "TPSProject.h"
#include "Enemy.h" //�ֳʹ� ���� �ޱ�����
#include "Kismet/GameplayStatics.h" //�Ͻ����� ����
#include "ObjectPools.h"
#include "Blueprint/UserWidget.h"
#include <PlayerAnim.h>


// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//�ʱ⼼�úκ�
	
	// ���� ������Ʈ
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// ����� ���� ������Ʈ �Ҵ�

	playerFire1 = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));//������ ���׶��� ��������

	
	ObjectPool = CreateDefaultSubobject<UObjectPools>(TEXT("OBJPool"));
	 
	//���̷��� �ҷ�����
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/Ch18_nonPBR.Ch18_nonPBR'"));
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

	gunMeshCompRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshCompRight"));
	gunMeshCompLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshCompLeft"));

	gunMeshCompRight->SetupAttachment(GetCapsuleComponent()); //���� ��ü�� ����
	gunMeshCompLeft->SetupAttachment(GetCapsuleComponent()); //���� ��ü�� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshCompRight->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshCompLeft->SetSkeletalMesh(TempGunMesh.Object);
		
		gunMeshCompRight->SetRelativeLocation(FVector(0, 80, 11));
		gunMeshCompLeft->SetRelativeLocation(FVector(0, -80, 11));
		
		gunMeshCompRight->SetRelativeRotation(FRotator(0, 0, 270));
		gunMeshCompLeft->SetRelativeRotation(FRotator(0, 0, 270));

	}

	//�������� �Ž��������
	sniperMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunMeshComp"));

	sniperMeshComp->SetupAttachment(GetMesh(),TEXT("hand_rSoket"));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/sniper1.sniper1'"));

	if (TempSniperMesh.Succeeded())
	{
		sniperMeshComp->SetSkeletalMesh(TempSniperMesh.Object);// �Ž� �Ҵ��ϱ�

		sniperMeshComp->SetRelativeLocation(FVector(4, -42, -2));//��ġ ����

		sniperMeshComp->SetRelativeRotation(FRotator(270, 96.38, 90));
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));//�Ž� ũ������
	}

	

	
	
}

//void ATPSPlayer::TestFunc(FName name)
//{
//	UE_LOG(LogTemp, Warning, TEXT("TestFunc Call : %s"), *name.ToString());
//}
//
//void ATPSPlayer::TestFunc2(FName name)
//{
//	UE_LOG(LogTemp, Warning, TEXT("MultiFunc Call : %s"), *name.ToString());
//}


// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//�ʱ� ü�� ����

	hp = initiaHp;

	
	HitIndex = FMath::RandRange(0, 3);
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSPlayer::OnEnemyKickOverlap); //������ ��������Ʈ ž��
	
	FTimerHandle Hidden;

	GetWorld()->GetTimerManager().SetTimer(Hidden, this, &ATPSPlayer::PlayerMeshBlink, 0.1f, true);
	// ���� ������Ʈ
	//playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// ����� ���� ������Ʈ �Ҵ�

	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));

	//�ʱ� �ӵ� �ȱ�� ���� //������Ʈ �̵����� ����
	//GetCharacterMovement()->MaxWalkSpeed = walkSpeed;


	//�������� ��������

	//��������Ʈ�� ���ε�

	//myVar.BindUObject(this, &ATPSPlayer::TestFunc);

//	myMVar.AddUObject(this, &ATPSPlayer::TestFunc);
	//myMVar.AddUObject(this, &ATPSPlayer::TestFunc2);


	//FTimerHandle timer;

	//GetWorld()->GetTimerManager().SetTimer(timer, this, &ATPSPlayer::PlayDelegate, 2);
	


}

//void  ATPSPlayer::PlayDelegate()
//{
//	int c = FMath::RandRange(1, 3);
//
//	if (c == 1)
//	{
//		myVar.ExecuteIfBound(TEXT("first"));
//	}
//	else if (c == 2)
//	{
//		myMVar.Broadcast(TEXT("second"));
//	}
//	else
//	{
//		myMVar.Broadcast(TEXT("SSS"));
//	}
//}
// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ������Ʈ�� �̵�
	//Move();
	
	if (isHit == true)
	{
		PlayerhitTimeCheck();
		//GetWorld()->GetTimerManager().SetTimer()
		//PlayerMeshBlink();

	}
	
		



}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//������Ʈ ���� �Է� ���ε� ó���ϵ��� �ϱ�

	onInputBindingDelegate.Broadcast(PlayerInputComponent);
	//playerMove->SetupInputBinding(PlayerInputComponent);
//	playerFire->SetupInputBinding(PlayerInputComponent);




}




void ATPSPlayer::OnEnemyKickOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	

	if (enemy != nullptr)
	{
	

			onHitEvent();
		
	}

}

void ATPSPlayer::UseGrenadeGun()
{
	gunMeshCompRight->SetVisibility(true);
	gunMeshCompLeft->SetVisibility(true);

	bUseingGrenadeGun = true;
}

void ATPSPlayer::PlayerhitTimeCheck()
{
	
	currentTime += GetWorld()->DeltaTimeSeconds;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerHit"));

	if (currentTime > damageDelayTime) {
	
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCollision"));
		isHit = false;
		currentTime = 0;
		GetMesh()->SetVisibility(true);



	}

}

void ATPSPlayer::PlayerMeshBlink()
{
	if (isHit) {
		if (GetMesh()->IsVisible() == false) {
			//GetMesh()->bHiddenInGame = true;
			GetMesh()->SetVisibility(true);
			UE_LOG(LogTemp, Warning, TEXT("hidden"));
		}
		else
		{
			GetMesh()->SetVisibility(false);
			UE_LOG(LogTemp, Warning, TEXT("Nohidden"));
		}
	}
}









void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	

}

void ATPSPlayer::onHitEvent()
{
	PRINT_LOG(TEXT("Damaged!!!!"));


	hp--;

	isHit = true;
	int32 index = FMath::RandRange(0, 3);
	FString sectionName = FString::Printf(TEXT("Hit%d"), index);
	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	anim->PlayerHitAnim(*sectionName);

	//PlayerhitTimeCheck();

	if (hp <= 0)
	{
		PRINT_LOG(TEXT("player Dead"));
		OnGameOver();
	}
}


