// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	m_socket = new ClientSocket();

	CS_LOGIN_PACKET packet;
	packet.size = sizeof(CS_LOGIN_PACKET);
	packet.type = PacketType::CS_LOGIN;
	FString name = TEXT("test");
	strcpy_s(packet.name, TCHAR_TO_UTF8(*name));
	m_socket->SendPacket(&packet);
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}