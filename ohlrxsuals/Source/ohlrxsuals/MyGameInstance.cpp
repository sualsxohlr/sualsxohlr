// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "PacketManager.h"

UMyGameInstance::UMyGameInstance()
{
	//m_socket = new ClientSocket();
}

void UMyGameInstance::Init()
{
	Super::Init();

	PacketManager::GetInstance()->Init();
}