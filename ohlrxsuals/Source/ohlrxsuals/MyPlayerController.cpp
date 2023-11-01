// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "PacketManager.h"
#include "MyCharacter.h"

AMyPlayerController::AMyPlayerController()
{
	UE_LOG(LogTemp, Log, TEXT("PlayerController"));

	PacketManager::GetInstance()->SetPlayerController(this);

	SCLoginDelegate.BindUObject(this, &AMyPlayerController::SCLoginInfoHandler);
	SCMovePlayerDelegate.BindUObject(this, &AMyPlayerController::SCMovePlayerHandler);

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

	SendPlayerInfo();
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("Exit"), EInputEvent::IE_Pressed, this, &AMyPlayerController::OnGameExited);
}

void AMyPlayerController::OnGameExited()
{
	CS_DISCONNECT_PLAYER_PACKET p;
	p.size = sizeof(CS_DISCONNECT_PLAYER_PACKET);
	p.type = PacketType::CS_DISCONNECT_PLAYER;	
	m_socket->SendPacket(&p);
	// Broadcast
}

void AMyPlayerController::SendPlayerInfo()
{
	CS_PLAYER_INFO_PACKET p;
	p.size = sizeof(CS_PLAYER_INFO_PACKET);
	p.type = PacketType::CS_PLAYER_INFO;

	FVector location = m_playerCharacter->GetActorLocation();
	FRotator rotation = m_playerCharacter->GetActorRotation();
	p.x = location.X;
	p.y = location.Y;
	p.z = location.Z;

	p.yaw = rotation.Yaw;
	p.pitch = rotation.Pitch;
	p.roll = rotation.Roll;

	m_socket->SendPacket(&p);
}

void AMyPlayerController::EnterPlayer(uint32 id, FVector location, FRotator rotation)
{
	//GetWorld()->SpawnActor<AMyCharacter>(location, rotation);
	UE_LOG(LogTemp, Log, TEXT("id Spawned !"), id);
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
	UE_LOG(LogTemp, Log, TEXT("[Login Info] ID : %d (%d, %d, %d) (%d, %d, %d)"), id, x, y, z, yaw, pitch, roll);

	FVector location(x, y, z);
	FRotator rotation(yaw, pitch, roll);
	EnterPlayer(id, location, rotation);
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

	UE_LOG(LogTemp, Log, TEXT("[Move] ID : %d (%d, %d, %d) (%d, %d, %d)"), id, x, y, z, yaw, pitch, roll);
}