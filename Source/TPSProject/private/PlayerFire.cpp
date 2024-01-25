// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "EnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include <PlayerAnim.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "ObjectPools.h"
#include "Math/Rotator.h"


FTimerHandle MemberTimerHandle;

UPlayerFire::UPlayerFire()
{
	//ConstructorHelpers::FObjectFinder<USoundBase>tempSound(TEXT("/Script/Engine.SoundWave'/Game/SniperGun/Rifle.Rifle'"));// ��λ� �ִ� ���� ���� �� ��������

	//if (tempSound.Succeeded()) //������
	//{
	//	bulletsound = tempSound.Object;  //�����ص� �Ѿ� ���忡 �־���
	//}


	
	//ConstructorHelpers::FObjectFinder<UUserWidget>tempSniperUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/WBP_SniperUI.WBP_SniperUI'"));
	//sniperUIFactory = tempSniperUI.Object;

	//ConstructorHelpers::FObjectFinder<UUserWidget>tempcrosshairUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/WBP_Crosshair.WBP_Crosshair'"));
	//crosshairUIFactory = tempcrosshairUI.Object;
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();
	tpsCamComp = me->tpsCamComp;
	gunMeshCompRight = me->gunMeshCompRight;
	gunMeshCompLeft = me->gunMeshCompLeft;

	sniperMeshComp = me->sniperMeshComp;

	//�������� ��������
	//_sniperUI =  sniperUIFactory;// ���忡�� �������� UI ������ UI �ν���Ʈ�� ��������

	//_crosshairUI = crosshairUIFactory;


	ChangeSniperGun();// ���� �������۷ν���

	bSniperAim = false;
	//crosshairUIFactory->AddToViewport();
	//_crosshairUI->AddToViewport();
	

	//�Ѿ˹߻�� �ð�������
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UPlayerFire::DelayGrenadeGun, GrenadeShotDelay);
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFire::InputFire);
	//���߿� Ű���帻�� �ر����� �����ؾ���
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &UPlayerFire::UseGrenadeGun);

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
			//bulletTrans.SetLocation( //Ʈ�������� ���������� ����

				//bulletTrans+=


				//FQuat::RotateVector(hitInfo.Normal);
				//FRotator::Quaternion(hitInfo.ImpactNormal);

				
			
			//bulletTrans.SetRotation(FQuat::Vector(hitInfo.ImpactNormal));
			//bulletTrans.SetRotation(); //Ʈ�������� ���������� ����

			

			//����Ʈ �����̼� �����ϱ�
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectBlood, bulletTrans); //�װ��� ����Ʈ ����
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),bulletEffectBlood,hitInfo.ImpactPoint,  FVector(1, 1, 1), true); //�װ��� ����Ʈ ����

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

				enemyFSM->OnDamageProcess(sniper_Damage);



			}
		}

	
}

void UPlayerFire::UseGrenadeGun()
{
	bUseingGrenadeGun = true;
	gunMeshCompRight->SetVisibility(true);
	gunMeshCompLeft->SetVisibility(true);

	sniperMeshComp->SetVisibility(false);

	me->OnUsingGrenade(bUseingGrenadeGun);
}

void UPlayerFire::ChangeSniperGun()
{
	bUseingGrenadeGun = false;
	gunMeshCompRight->SetVisibility(false);
	gunMeshCompLeft->SetVisibility(false);
	sniperMeshComp->SetVisibility(true);

	me->OnUsingGrenade(bUseingGrenadeGun);
}

void UPlayerFire::SniperZoom()
{
	if (bUseingGrenadeGun) {
		return;
	}

	if (bSniperAim) {

		bSniperAim = false;
		//_sniperUI->RemoveFromParent(); // UI�� ����Ʈ�� ���̴°��� ����
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(90.0f); // ī�޶� �並 90���� ���� �ܾƿ�

		//_crosshairUI->AddToViewport();
	}
	else
	{
		bSniperAim = true;
		//_sniperUI->AddToViewport();		// ��������UI�� ����Ʈ�� ���
		check(tpsCamComp);
		tpsCamComp->SetFieldOfView(45.0f); // ī�޶� �並 45.0�� ���� ����

		//_crosshairUI->RemoveFromParent();

	}
}

void UPlayerFire::DelayGrenadeGun()
{
	
		if (bUseingGrenadeGun) {
			FTransform firePosRight = gunMeshCompRight->GetSocketTransform(TEXT("FirePosition"));
			FTransform firePosLeft = gunMeshCompLeft->GetSocketTransform(TEXT("FirePosition"));


			me->ObjectPool->SpawnPooledObject()->SetActorRelativeTransform(firePosRight);
			me->ObjectPool->SpawnPooledObject()->SetActorRelativeTransform(firePosLeft);
		}
		GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UPlayerFire::DelayGrenadeGun, GrenadeShotDelay);
}

void UPlayerFire::OnShotBullet_Implementation()
{

}


