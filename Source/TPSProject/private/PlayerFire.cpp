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


FTimerHandle MemberTimerHandle; //������ Ÿ�̸�

UPlayerFire::UPlayerFire()
{
	//��� ����Ʈ���� ���� ����

}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshCompRight = me->gunMeshCompRight;
	gunMeshCompLeft = me->gunMeshCompLeft;

	

	mes = Cast<ATPSPlayer>(GetOwner());
	

	bSniperAim = false;

	//�Ѿ˹߻�� �ð�������
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UPlayerFire::DelayGrenadeGun, SubGunShotDelay);

	gunMeshCompRight->SetVisibility(false);
	gunMeshCompLeft->SetVisibility(false);
	//������ ��Ȱ��
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &UPlayerFire::SniperZoom);

}


void UPlayerFire::InputFire()
{

	//�ִϸ��̼� ����
	auto anim = Cast<UPlayerAnim>(mes->GetMesh()->GetAnimInstance());
	anim->PlayAttckAnim();// �޾ƿͼ� ��ɽ���

	//ī�޶� ����ũ ����

	auto controller = GetWorld()->GetFirstPlayerController();
	//���� ī�޶� ����ũ �������Ʈ�� �־ ����
	controller->PlayerCameraManager->StartCameraShake(cameraShake);


	//���� ����
	UGameplayStatics::PlaySound2D(GetWorld(), bulletsound);

	//��Ÿ�Ե��� ����
	
		//�������� �ѹ߻� ����Ʈ���̼� ���
		// ī�޶���ۺ���
		FVector startPos = tpsCamComp->GetComponentLocation();
		// ī�޶��� �ձ���
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;

		FHitResult hitInfo; // �����޾ƿ���

		FCollisionQueryParams params; // �浹�ɼ�?

		params.AddIgnoredActor(me); // �ڱ��ڽ��� �����ϵ��� ����

		// ������ �ϳ����� �Ѱ��� �������� (������, ���۰� ��, ä��, �浹�ɼ�)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		// �¾Ҵ��� ����
		if (bHit) {

			FTransform bulletTrans; //��ġ ������� Ʈ������

			bulletTrans.SetLocation(hitInfo.ImpactPoint+hitInfo.ImpactNormal); //Ʈ�������� ���������� ����
			

			//����Ʈ �����̼� �����ϱ�
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectBlood, bulletTrans); //�װ��� ����Ʈ ����
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),bulletEffectBlood,hitInfo.ImpactPoint,  FVector(1, 1, 1), true); //�װ��� ����Ʈ ����

			auto hitComp = hitInfo.GetComponent();
			// �������� ������Ʈ ��������
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			auto itemBox = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("Hit"));

			if (hitComp && hitComp->IsSimulatingPhysics()) //�������� �ְ� �������� �������밡���ϴٸ� 
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectDefault, bulletTrans);
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 50000; // ���� ǥ�鿡 ���� �ݴ�������� ���� 

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
			

			//�ε��� ����� ���� ������ �Ǵ��ϱ�
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
		tpsCamComp->SetFieldOfView(90.0f); // ī�޶� �並 90���� ���� �ܾƿ�

	}
	else
	{
		bSniperAim = true;
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(45.0f); // ī�޶� �並 45.0�� ���� ����

	

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


