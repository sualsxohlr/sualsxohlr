// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "ClientSocket.h"

UMyGameInstance::UMyGameInstance()
{
	ClientSocket* socket = new ClientSocket();
}

void UMyGameInstance::Init()
{
	Super::Init();
	// UE_LOG(LogTemp, Error, TEXT("Init Completed"));
}