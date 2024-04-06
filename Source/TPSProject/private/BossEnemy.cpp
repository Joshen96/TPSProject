// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TPSPlayer.h"

// Sets default values
ABossEnemy::ABossEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Blueprints/Boss/BossAnim/Bossfile/Mutant_Walking.Mutant_Walking'"));
	//매쉬가져오고
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));

		GetMesh()->SetRelativeScale3D(FVector(0.84f));

	}

	bossfsm1 = CreateDefaultSubobject<UBossFSM>(TEXT("BOSSFSM"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());

	//플레이어 액터 담아주고
	//target = Cast<ATPSPlayer>(actor);
	
	
	
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossEnemy::DestoryEnemy()
{
	Destroy();
}

void ABossEnemy::LaunchForward(float LaunchSpeed)
{
	// 캐릭터 이동 방향 설정
	FVector LaunchDirection = GetActorForwardVector(); // 현재 캐릭터의 앞 방향 벡터를 가져옴
	LaunchDirection.Normalize(); // 벡터를 정규화하여 길이를 1로 만듦

	// 캐릭터 이동
	LaunchCharacter(LaunchDirection * LaunchSpeed, false, false); // 캐릭터를 지정된 속도와 방향으로 띄움
}

void ABossEnemy::LookatTarget()
{

	

	FRotator Lookat = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation());
	Lookat.Pitch = 0; //위아래 잠금 pitch 회전잠금
	SetActorRotation(Lookat);
}


