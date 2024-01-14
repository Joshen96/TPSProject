// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyFSM.h"
#include "Enemy.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//1. �浹ü ���
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	
	//2. �浹 �������� ����
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	//3. �浹ü ũ�� ����
	collisionComp->SetSphereRadius(13);

	//4. ��Ʈ�� ���
	RootComponent = collisionComp;


	//�ܰ����
	//1. �Ž�������Ʈ ����ϱ�
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	//2. �θ�������Ʈ ����
	bodyMeshComp->SetupAttachment(collisionComp);
	//3. �Ž��� �⺻ �浹 ��Ȱ��ȭ
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//4. �Ž� ũ�⼳��
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));


	//�߻�ü ������Ʈ
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//1. movement ������Ʈ�� ���Ž�ų ������Ʈ ����

	movementComp->SetUpdatedComponent(collisionComp);
	//2. �ʱ�ӵ�
	movementComp->InitialSpeed = 5000;

	//3. �ִ�ӵ�
	movementComp->MaxSpeed = 5000;

	//4 �ݵ�����
	movementComp->bShouldBounce = true;//�ݵ�

	//5. �ݵ���
	movementComp->Bounciness = 0.3f;

	//InitialLifeSpan = 2.0f; // �����ֱ�
	

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	//�ð����� �̱��� ���� ������ ó���ϴ¹��
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnEnemyTouch); // �ൿ�� �Լ� ��������Ʈ ž��
	
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die()
{
	Destroy();
}

void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}



void ABullet::OnEnemyTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	auto enemy = OtherActor->GetDefaultSubobjectByName(TEXT("FSM"));

	if (enemy) {
		auto enemyFSM = Cast<UEnemyFSM>(enemy);

		enemyFSM->OnDamageProcess();
		UE_LOG(LogTemp, Warning, TEXT("touch"));


	}

	
}





