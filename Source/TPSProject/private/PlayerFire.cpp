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
	ConstructorHelpers::FObjectFinder<USoundBase>tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));// ��λ� �ִ� ���� ���� �� ��������

	if (tempSound.Succeeded()) //������
	{
		bulletsound = tempSound.Object;  //�����ص� �Ѿ� ���忡 �־���
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperMeshComp = me->sniperMeshComp;

	//�������� ��������
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);// ���忡�� �������� UI ������ UI �ν���Ʈ�� ��������

	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);


	ChangeSniperGun();// ���� �������۷ν���
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

	//�ִϸ��̼� ����
	auto anim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttckAnim();// �޾ƿͼ� ��ɽ���

	//ī�޶� ����ũ ����

	auto controller = GetWorld()->GetFirstPlayerController();
	//���� ī�޶� ����ũ �������Ʈ�� �־ ����
	controller->PlayerCameraManager->StartCameraShake(cameraShake);


	//���� ����
	UGameplayStatics::PlaySound2D(GetWorld(), bulletsound);

	//��Ÿ�Ե��� ����
	if (bUseingGrenadeGun) {
		FTransform firePos = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));

		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePos);

	}
	else
	{
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

			bulletTrans.SetLocation(hitInfo.ImpactPoint); //Ʈ�������� ���������� ����

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans); //�װ��� ����Ʈ ����

			auto hitComp = hitInfo.GetComponent();
			// �������� ������Ʈ ��������

			if (hitComp && hitComp->IsSimulatingPhysics()) //�������� �ְ� �������� �������밡���ϴٸ� 
			{
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 50000; // ���� ǥ�鿡 ���� �ݴ�������� ���� 

				hitComp->AddForce(force);
			}

			//�ε��� ����� ���� ������ �Ǵ��ϱ�

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
