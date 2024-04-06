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
	//�Ž���������
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

	//�÷��̾� ���� ����ְ�
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
	// ĳ���� �̵� ���� ����
	FVector LaunchDirection = GetActorForwardVector(); // ���� ĳ������ �� ���� ���͸� ������
	LaunchDirection.Normalize(); // ���͸� ����ȭ�Ͽ� ���̸� 1�� ����

	// ĳ���� �̵�
	LaunchCharacter(LaunchDirection * LaunchSpeed, false, false); // ĳ���͸� ������ �ӵ��� �������� ���
}

void ABossEnemy::LookatTarget()
{

	

	FRotator Lookat = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation());
	Lookat.Pitch = 0; //���Ʒ� ��� pitch ȸ�����
	SetActorRotation(Lookat);
}


