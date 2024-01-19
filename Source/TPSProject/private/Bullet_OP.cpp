// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_OP.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyFSM.h"
#include "Enemy.h"



ABullet_OP::ABullet_OP()
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
	//movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//1. movement ������Ʈ�� ���Ž�ų ������Ʈ ����

	//movementComp->SetUpdatedComponent(collisionComp);
	//2. �ʱ�ӵ�
	//movementComp->InitialSpeed = 5000;

	//3. �ִ�ӵ�
	//movementComp->MaxSpeed = 5000;

	//4 �ݵ�����
	//movementComp->bShouldBounce = true;//�ݵ�

	//5. �ݵ���
	//movementComp->Bounciness = 0.3f;

	//InitialLifeSpan = 2.0f; // �����ֱ�

	
}

void ABullet_OP::BeginPlay()
{
	Super::BeginPlay();
	//�ð����� �̱��� ���� ������ ó���ϴ¹��
	//FTimerHandle deathTimer;
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet_OP::Die, 2.0f, false);

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_OP::OnEnemyTouch); // �ൿ�� �Լ� ��������Ʈ ž��


	//SetLifeSpan(LifeTime);
}

void ABullet_OP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FrontShot(DeltaTime);

}

void ABullet_OP::Die()
{
	Destroy();
}



//void ABullet_OP::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
//	{
//		movementComp->InitialSpeed = speed;
//		movementComp->MaxSpeed = speed;
//	}
//
//}

void ABullet_OP::OnEnemyTouch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	auto enemy = OtherActor->GetDefaultSubobjectByName(TEXT("FSM"));

	if (enemy) {
		auto enemyFSM = Cast<UEnemyFSM>(enemy);


		enemyFSM->OnDamageProcess(bulletDamage);
		UE_LOG(LogTemp, Warning, TEXT("touch"));


	}
}

void ABullet_OP::FrontShot(float DeltaTime)
{
	FVector newLocation = GetActorLocation() + GetActorForwardVector() * speed * DeltaTime;

	SetActorLocation(newLocation);

}





//void ABullet_OP::SimulationEnabled(bool type)
//{
//	movementComp->bSimulationEnabled = type;
//}
//
//void ABullet_OP::Deactivate()
//{
//	Super::Deactivate();
//	SimulationEnabled(false);
//
//}

