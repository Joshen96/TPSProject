// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//1. 충돌체 등록
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	
	//2. 충돌 프로파일 설정
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	//3. 충돌체 크기 설정
	collisionComp->SetSphereRadius(13);

	//4. 루트로 등록
	RootComponent = collisionComp;


	//외관등록
	//1. 매쉬컴포넌트 등록하기
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	//2. 부모컴포넌트 지정
	bodyMeshComp->SetupAttachment(collisionComp);
	//3. 매쉬에 기본 충돌 비활성화
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//4. 매쉬 크기설정
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));


	//발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	//1. movement 컴포넌트가 갱신시킬 컴포넌트 지정

	movementComp->SetUpdatedComponent(collisionComp);
	//2. 초기속도
	movementComp->InitialSpeed = 5000;

	//3. 최대속도
	movementComp->MaxSpeed = 5000;

	//4 반동여부
	movementComp->bShouldBounce = true;//반동

	//5. 반동값
	movementComp->Bounciness = 0.3f;

	//InitialLifeSpan = 2.0f; // 생명주기
	

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	//시간관리 싱글톤 으로 몇초후 처리하는방법
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);
	
	
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

