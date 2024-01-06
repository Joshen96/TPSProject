// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComponent.h"

// Sets default values for this component's properties
//UPlayerBaseComponent::UPlayerBaseComponent()
//{
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	PrimaryComponentTick.bCanEverTick = true;
//
//	// ...
//}


void UPlayerBaseComponent::InitializeComponent()
{

	me = Cast<ATPSPlayer>(GetOwner()); // 캐스트 이컴포넌트가 달린 것에서 캐스트

	moveComp = me->GetCharacterMovement(); // 무브컴포넌트 할당

	//여기서 플레이어 본체에 있는 델리게이트 바인딩

	//me->onInputBindingDelegate.AddUObject()
	me->onInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);

}

// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

