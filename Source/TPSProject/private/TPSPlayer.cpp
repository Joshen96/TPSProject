// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerMove.h" //이동컴포넌트
#include "PlayerFire.h" //공격 컴포넌트
#include "Components/CapsuleComponent.h"
#include "TPSProject.h"
#include "Enemy.h" //애너미 공격 받기위해
#include "Kismet/GameplayStatics.h" //일시정지 위함
#include "ObjectPools.h"
#include "Blueprint/UserWidget.h"
#include <PlayerAnim.h>


// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//초기세팅부분
	
	// 무브 컴포넌트
	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// 사용할 공격 컴포넌트 할당

	playerFire1 = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));//생성자 버그때매 블프구현

	
	ObjectPool = CreateDefaultSubobject<UObjectPools>(TEXT("OBJPool"));
	 
	//스켈레톤 불러오기
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/Ch18_nonPBR.Ch18_nonPBR'"));
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

	gunMeshCompRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshCompRight"));
	gunMeshCompLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshCompLeft"));

	gunMeshCompRight->SetupAttachment(GetCapsuleComponent()); //총을 몸체에 부착
	gunMeshCompLeft->SetupAttachment(GetCapsuleComponent()); //총을 몸체에 부착
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

	//스나이퍼 매쉬컴포등록
	sniperMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunMeshComp"));

	sniperMeshComp->SetupAttachment(GetMesh(),TEXT("hand_rSoket"));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/sniper1.sniper1'"));

	if (TempSniperMesh.Succeeded())
	{
		sniperMeshComp->SetSkeletalMesh(TempSniperMesh.Object);// 매시 할당하기

		sniperMeshComp->SetRelativeLocation(FVector(4, -42, -2));//위치 조절

		sniperMeshComp->SetRelativeRotation(FRotator(270, 96.38, 90));
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));//매쉬 크기조정
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

	//초기 체력 설정

	hp = initiaHp;

	
	HitIndex = FMath::RandRange(0, 3);
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSPlayer::OnEnemyKickOverlap); //적공격 델리게이트 탑재
	
	FTimerHandle Hidden;

	GetWorld()->GetTimerManager().SetTimer(Hidden, this, &ATPSPlayer::PlayerMeshBlink, 0.1f, true);
	// 무브 컴포넌트
	//playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	// 사용할 공격 컴포넌트 할당

	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));

	//초기 속도 걷기로 세팅 //컴포넌트 이동으로 삭제
	//GetCharacterMovement()->MaxWalkSpeed = walkSpeed;


	//스나이퍼 위젯설정

	//델리게이트에 바인드

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

	// 움직임 컴포넌트로 이동
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
	//컴포넌트 에서 입력 바인딩 처리하도록 하기

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


