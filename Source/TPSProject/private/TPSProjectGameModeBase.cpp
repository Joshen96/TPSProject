// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSProjectGameModeBase.h"
#include "TPSProject.h"

ATPSProjectGameModeBase::ATPSProjectGameModeBase()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *CALLINFO);

	//PRINT_CALLINFO();

	PRINT_LOG(TEXT("My Log : %s"), TEXT("TPS project!!"));
}
