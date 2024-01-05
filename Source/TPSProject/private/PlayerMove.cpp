// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	moveComp->MaxWalkSpeed = walkSpeed; //�ʱ� �ӵ� ����
}


void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);//������Ʈ ����ϱ����� Move

	Move();
}


void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value); //��ȸ������ �Է°� ����
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value); //��ġ������ �Է°� ����
}


void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	//���콺 �Է� ���ε� ��
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);

	//Ű���� �̵� �յ��¿� ���ε� ��
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);

	//Ű���� ���� ���ε� �׼� 
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);

	//Ű���� �� ���ε� �׼� 
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);

	
}


void UPlayerMove::InputHorizontal(float value)
{
	dir.Y = value;
}

void UPlayerMove::InputVertical(float value)
{
	dir.X = value;
}

void UPlayerMove::InputJump()
{
	me->Jump();
}


void UPlayerMove::Move()
{
	dir = FTransform(me->GetControlRotation()).TransformVector(dir); //������ǥ�� �����ǥ �������� �ٲ���
	// ���� �̵�
	// ������ġ + ���� * �ӵ� * �ð� =  �����ġ
	/*FVector P;

	FVector NowL = GetActorLocation();

	FVector vt = dir * moveSpeed * DeltaTime;

	P = vt + NowL;

	SetActorLocation(P);*/
	// ��ü�ϱ�
	me->AddMovementInput(dir);

	dir = FVector::ZeroVector;
}


void UPlayerMove::InputRun()
{
	auto movement = moveComp;

	if (movement->MaxWalkSpeed > walkSpeed)
	{
		movement->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = runSpeed;
	}
}
