// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "PacketManager.h"

AMyPlayerController::AMyPlayerController()
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PacketManager::GetInstance()->SetPlayerController(this);

	SCLoginDelegate.BindUObject(this, &AMyPlayerController::SCLoginInfoHandler);
	SCMovePlayerDelegate.BindUObject(this, &AMyPlayerController::SCMovePlayerHandler);

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

void AMyPlayerController::EnterPlayer(uint32 id, FVector location, FRotator rotation)
{
	//ACharacter* newPlayer = GetWorld()->SpawnActor<ACharacter>(location, rotation);
	UE_LOG(LogTemp, Log, TEXT("id ½ºÆùµÊ !"), id);
}

void AMyPlayerController::SCLoginInfoHandler(char* packet)
{
	SC_LOGIN_INFO_PACKET* p = reinterpret_cast<SC_LOGIN_INFO_PACKET*>(packet);
	uint32 id = p->id;
	float x = p->x;
	float y = p->y;
	float z = p->z;
	
	float yaw = p->yaw;
	float pitch = p->pitch;
	float roll = p->roll;
	UE_LOG(LogTemp, Log, TEXT("[Login Info] ID : %d , X : %d, Y : %d, Z: %d"), id, x, y, z);

	FVector location(x, y, z);
	EnterPlayer(id, location, FRotator::ZeroRotator);
}

void AMyPlayerController::SCMovePlayerHandler(char* packet)
{
	SC_MOVE_PLAYER_PACKET* p = reinterpret_cast<SC_MOVE_PLAYER_PACKET*>(packet);
	
	uint32 id = p->id;

	float x = p->x;
	float y = p->y;
	float z = p->z;

	float yaw = p->yaw;
	float pitch = p->pitch;
	float roll = p->roll;
	UE_LOG(LogTemp, Log, TEXT("[Move] ID : %d , X : %d, Y : %d, Z: %d"), id, x, y, z);
}