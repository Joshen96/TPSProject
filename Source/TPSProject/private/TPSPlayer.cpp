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
	//초기세팅부분
	// 무브 컴포넌트
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	
	 
	//스켈레톤 불러오기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
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
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSoket")); //총을 몸체에 부착
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);

		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	//스나이퍼 매쉬컴포등록
	sniperMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunMeshComp"));

	sniperMeshComp->SetupAttachment(GetMesh(),TEXT("hand_rSoket"));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/sniper1.sniper1'"));

	if (TempSniperMesh.Succeeded())
	{
		sniperMeshComp->SetSkeletalMesh(TempSniperMesh.Object);// 매시 할당하기

		sniperMeshComp->SetRelativeLocation(FVector(-22, 55, 120));//위치 조절

		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));//매쉬 크기조정
	}

	
	
}


// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//초기 속도 걷기로 세팅 //컴포넌트 이동으로 삭제
	//GetCharacterMovement()->MaxWalkSpeed = walkSpeed;


	//스나이퍼 위젯설정
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);// 월드에서 스나이퍼 UI 정보를 UI 인스턴트에 생성해줌

	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);


	ChangeSniperGun();// 시작 스나이퍼로시작
	bSniperAim = false;
	_crosshairUI->AddToViewport();


	


}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 움직임 컴포넌트로 이동
	//Move();

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//컴포넌트 에서 입력 바인딩 처리하도록 하기
	playerMove->SetupInputBinding(PlayerInputComponent);






	//입력 매핑하기
	//카메라 움직임 컴포넌트를 만들어 따로 동작하도록 변경
	//PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	//PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	//PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	//PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	////바인딩 액션은 IE_Pressed 처럼 눌러졌을때를 정해줘야함
	//PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &ATPSPlayer::InputJump);
	
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeGrenadeGun);
	
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeSniperGun);

	
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperZoom);
	
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperZoomOut); //버그로 변경

	


}

void ATPSPlayer::InputFire()
{
	//애니메이션 실행
	 auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	 anim->PlayAttckAnim();// 받아와서 기능실행
	
	 //카메라 쉐이크 실행

	 auto controller = GetWorld()->GetFirstPlayerController();
	 //만든 카메라 쉐이크 블루프린트를 넣어서 적용
	 controller->PlayerCameraManager->StartCameraShake(cameraShake);


	 //사운드 실행
	 UGameplayStatics::PlaySound2D(GetWorld(), bulletsound);

	//총타입따라 생성
	if (bUseingGrenadeGun) {
		FTransform firePos = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePos);

	}
	else
	{
		//스나이퍼 총발사 라인트레이서 사용
		// 카메라시작부터
		FVector startPos = tpsCamComp->GetComponentLocation();
		// 카메라의 앞까지
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;

		FHitResult hitInfo; // 정보받아오기

		FCollisionQueryParams params; // 충돌옵션?

		params.AddIgnoredActor(this); // 자기자신은 무시하도록 설정
		
		// 라인을 하나쏴서 한개만 가져오기 (맞은곳, 시작과 끝, 채널, 충돌옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// 맞았는지 여부
		if (bHit) {

			FTransform bulletTrans; //위치 담기위한 트랜스폼

			bulletTrans.SetLocation(hitInfo.ImpactPoint); //트랜스폼을 맞은곳으로 변경

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory,bulletTrans); //그곳에 이펙트 구현

			auto hitComp = hitInfo.GetComponent(); 
			// 맞은것의 컴포넌트 가져오기

			if (hitComp && hitComp->IsSimulatingPhysics()) //맞은것이 있고 맞은것이 물리적용가능하다면 
			{
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 50000; // 맞은 표면에 힘의 반대방향으로 힘을 
			
				hitComp->AddForce(force);
			}

			//부딪힌 대상이 먼저 적인지 판단하기

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
//		_sniperUI->RemoveFromParent(); // UI를 뷰포트에 보이는것을 제거
//		tpsCamComp->SetFieldOfView(90.0f); // 카메라 뷰를 90으로 변경 줌아웃
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
		_sniperUI->AddToViewport();		// 스나이퍼UI를 뷰포트에 출력
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(45.0f); // 카메라 뷰를 45.0로 변경 줌인
		
		_crosshairUI->RemoveFromParent();
		
	}
	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent(); // UI를 뷰포트에 보이는것을 제거
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(90.0f); // 카메라 뷰를 90으로 변경 줌아웃
		
		_crosshairUI->AddToViewport();
	}

	
}
