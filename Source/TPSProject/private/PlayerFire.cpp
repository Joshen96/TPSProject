// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnemyFSM.h"
#include "ItemBox.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include <PlayerAnim.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "ObjectPools.h"
#include "Math/Rotator.h"
#include "Kismet/KismetMathLibrary.h"


FTimerHandle MemberTimerHandle; //서브총 타이머

UPlayerFire::UPlayerFire()
{
	//블루 프린트에서 직접 세팅

}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshCompRight = me->gunMeshCompRight;
	gunMeshCompLeft = me->gunMeshCompLeft;

	

	mes = Cast<ATPSPlayer>(GetOwner());
	

	bSniperAim = false;

	//총알발사기 시간딜레이
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UPlayerFire::DelayGrenadeGun, SubGunShotDelay);

	gunMeshCompRight->SetVisibility(false);
	gunMeshCompLeft->SetVisibility(false);
	//서브총 비활성
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperZoom);

}


void UPlayerFire::InputFire()
{

	//애니메이션 실행
	auto anim = Cast<UPlayerAnim>(mes->GetMesh()->GetAnimInstance());
	anim->PlayAttckAnim();// 받아와서 기능실행

	//카메라 쉐이크 실행

	auto controller = GetWorld()->GetFirstPlayerController();
	//만든 카메라 쉐이크 블루프린트를 넣어서 적용
	controller->PlayerCameraManager->StartCameraShake(cameraShake);


	//사운드 실행
	UGameplayStatics::PlaySound2D(GetWorld(), bulletsound);

	//총타입따라 생성
	
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

			bulletTrans.SetLocation(hitInfo.ImpactPoint+hitInfo.ImpactNormal); //트랜스폼을 맞은곳으로 변경
			

			//이펙트 로테이션 세팅하기
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectBlood, bulletTrans); //그곳에 이펙트 구현
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),bulletEffectBlood,hitInfo.ImpactPoint,  FVector(1, 1, 1), true); //그곳에 이펙트 구현

			auto hitComp = hitInfo.GetComponent();
			// 맞은것의 컴포넌트 가져오기
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			auto itemBox = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("Hit"));

			if (hitComp && hitComp->IsSimulatingPhysics()) //맞은것이 있고 맞은것이 물리적용가능하다면 
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectDefault, bulletTrans);
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 50000; // 맞은 표면에 힘의 반대방향으로 힘을 

				hitComp->AddForce(force);
			}
			else if(enemy!=nullptr)
			{
				//auto enemyFSM = Cast<UEnemyFSM>(enemy);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectBlood, bulletTrans);

				//enemyFSM->OnDamageProcess(sniper_Damage);
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectGround, bulletTrans);

			}
			

			//부딪힌 대상이 먼저 적인지 판단하기
			if (itemBox) {
				auto item = Cast<AItemBox>(itemBox);
				//UE_LOG(LogTemp, Warning, TEXT("touch"));
				//GetWorld()->SpawnActor<AActor>(ItemcheckActor, bulletTrans, FRotator(0));

				GetWorld()->SpawnActor<AActor>(ItemcheckActor, bulletTrans);
					

			}else if (enemy) {
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				
				enemyFSM->OnDamageProcess(sniper_Damage);



			}
			else
			{

			}


		}

	
}




void UPlayerFire::SniperZoom()
{
	if (bSniperAim) 
	{

		bSniperAim = false;
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(90.0f); // 카메라 뷰를 90으로 변경 줌아웃

	}
	else
	{
		bSniperAim = true;
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(45.0f); // 카메라 뷰를 45.0로 변경 줌인

	

	}
}

void UPlayerFire::DelayGrenadeGun()
{
	
		if (me->bUseingGrenadeGun) {

			FTransform firePosRight = gunMeshCompRight->GetSocketTransform(TEXT("FirePosition"));
			FTransform firePosLeft = gunMeshCompLeft->GetSocketTransform(TEXT("FirePosition"));

			me->ObjectPool->SpawnPooledObject()->SetActorRelativeTransform(firePosRight);
			me->ObjectPool->SpawnPooledObject()->SetActorRelativeTransform(firePosLeft);
		}
		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UPlayerFire::DelayGrenadeGun, SubGunShotDelay);
}

void UPlayerFire::BasicGunDamageUP_Implementation()
{
	sniper_Damage *=1.2;

}

void UPlayerFire::OnShotBullet_Implementation()
{

}


