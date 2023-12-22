// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"

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
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
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
	

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh()); //총을 몸체에 부착
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	//스나이퍼 매쉬컴포등록
	sniperMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunMeshComp"));

	sniperMeshComp->SetupAttachment(GetMesh());


	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SniperGun/sniper1.sniper1'"));

	if (TempGunMesh.Succeeded())
	{
		sniperMeshComp->SetSkeletalMesh(TempGunMesh.Object);// 매시 할당하기

		sniperMeshComp->SetRelativeLocation(FVector(-22, 55, 120));//위치 조절

		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));//매쉬 크기조정
	}

}


// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);// 월드에서 스나이퍼 UI 정보를 UI 인스턴트에 생성해줌

	ChangeSniperGun();// 시작 스나이퍼로시작
	bSniperAim = false;
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
	//입력 매핑하기

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	//바인딩 액션은 IE_Pressed 처럼 눌러졌을때를 정해줘야함
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &ATPSPlayer::InputJump);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeGrenadeGun);
	
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeSniperGun);

	
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);


}

void ATPSPlayer::InputFire()
{
	FTransform firePos= gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePos);
	
	
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value); //요회전으로 입력값 적용
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value); //피치값으로 입력값 적용
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
	dir = FTransform(GetControlRotation()).TransformVector(dir); //절대좌표를 상대좌표 방향으로 바꿔줌
	// 실제 이동
	// 현재위치 + 방향 * 속도 * 시간 =  결과위치
	/*FVector P;

	FVector NowL = GetActorLocation();

	FVector vt = dir * moveSpeed * DeltaTime;

	P = vt + NowL;

	SetActorLocation(P);*/
	// 대체하기
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
	if (bSniperAim == false)
	{
		bSniperAim = true; //스나이퍼 에임 상태 변경
		_sniperUI->AddToViewport();		// 스나이퍼UI를 뷰포트에 출력ㄴ
		tpsCamComp->SetFieldOfView(45.0f); // 카메라 뷰를 45.0로 변경 줌인
	}
	else
	{
		bSniperAim = false; // 스나이퍼에임 상태 변경
		_sniperUI->RemoveFromParent(); // UI를 뷰포트에 보이는것을 제거
		tpsCamComp->SetFieldOfView(90.0f); // 카메라 뷰를 90으로 변경 줌아웃
	}
}

