// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	moveComp->MaxWalkSpeed = walkSpeed; //초기 속도 설정
}


void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);//업데이트 사용하기위함 Move

	Move();
}


void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value); //요회전으로 입력값 적용
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value); //피치값으로 입력값 적용
}


void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{

	//마우스 입력 바인딩 축
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);

	//키보드 이동 앞뒤좌우 바인딩 축
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);

	//키보드 점프 바인딩 액션 
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMove::InputJump);

	//키보드 런 바인딩 액션 
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
	dir = FTransform(me->GetControlRotation()).TransformVector(dir); //절대좌표를 상대좌표 방향으로 바꿔줌
	// 실제 이동
	// 현재위치 + 방향 * 속도 * 시간 =  결과위치
	/*FVector P;

	FVector NowL = GetActorLocation();

	FVector vt = dir * moveSpeed * DeltaTime;

	P = vt + NowL;

	SetActorLocation(P);*/
	// 대체하기
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
