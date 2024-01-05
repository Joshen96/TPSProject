// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include <PlayerAnim.h>
#include "GameFramework/CharacterMovementComponent.h"


UPlayerFire::UPlayerFire()
{
	ConstructorHelpers::FObjectFinder<USoundBase>tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));// 경로상에 있는 사운드 파일 을 꺼내오고

	if (tempSound.Succeeded()) //성공시
	{
		bulletsound = tempSound.Object;  //선언해둔 총알 사운드에 넣어줌
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperMeshComp = me->sniperMeshComp;

	//스나이퍼 위젯설정
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);// 월드에서 스나이퍼 UI 정보를 UI 인스턴트에 생성해줌

	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);


	ChangeSniperGun();// 시작 스나이퍼로시작
	bSniperAim = false;
	_crosshairUI->AddToViewport();
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);

	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::ChangeGrenadeGun);

	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &UPlayerFire::ChangeSniperGun);


	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperZoom);

}





void UPlayerFire::InputFire()
{

	//애니메이션 실행
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
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

		params.AddIgnoredActor(me); // 자기자신은 무시하도록 설정

		// 라인을 하나쏴서 한개만 가져오기 (맞은곳, 시작과 끝, 채널, 충돌옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// 맞았는지 여부
		if (bHit) {

			FTransform bulletTrans; //위치 담기위한 트랜스폼

			bulletTrans.SetLocation(hitInfo.ImpactPoint); //트랜스폼을 맞은곳으로 변경

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans); //그곳에 이펙트 구현

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

void UPlayerFire::ChangeGrenadeGun()
{
}

void UPlayerFire::ChangeSniperGun()
{
}

void UPlayerFire::SniperZoom()
{
}
