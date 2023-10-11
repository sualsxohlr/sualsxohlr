// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSocket.h"
#ifdef USE_FSOCKET
#include "Sockets.h"
#include "SocketSubSystem.h"
#include "Common/TcpSocketBuilder.h"
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

// #include "ohlrxsuals.h"
#include "PacketManager.h"

ClientSocket::ClientSocket()
{
	if (Connect()) {
		UE_LOG(LogTemp, Log, TEXT("Connected!"));
		m_thread = FRunnableThread::Create(this, TEXT("RecvThread"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Socket Connect Failed"));
		CleanUp();
	}
}

ClientSocket::~ClientSocket()
{
	if (m_thread != nullptr) {
		m_thread->WaitForCompletion();
		m_thread->Kill(true);
		delete m_thread;
	}
}

bool ClientSocket::Connect()
{
#ifdef USE_FSOCKET
	m_socket = FTcpSocketBuilder(TEXT("ClientSocket"));
	m_socket->SetNonBlocking(true);

	FString address = TEXT("127.0.0.1");
	FIPv4Address IpAddress;
	FIPv4Address::Parse(address, IpAddress);

	FIPv4Endpoint Endpoint = FIPv4Endpoint(IpAddress, PORT_NUM);
	return m_socket->Connect(*Endpoint.ToInternetAddr());
#else
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	m_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUM);
	FString address = TEXT("127.0.0.1");
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	uint32 ret = connect(m_socket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	if (ret == SOCKET_ERROR) return false;
	else return true;
#endif
}

void ClientSocket::CleanUp()
{
#ifdef USE_FSOCKET
	if (m_socket) {
		if (m_socket->GetConnectionState() == SCS_Connected) {
			m_socket->Close();
		}
		ISocketSubsystem* SocketSubSystem = ISocketSubsystem::Get();
		SocketSubSystem->DestroySocket(m_socket);

		m_socket = nullptr;
	}
#else

	CS_DISCONNECT_PLAYER_PACKET p;
	p.size = sizeof(CS_DISCONNECT_PLAYER_PACKET);
	p.type = PacketType::CS_DISCONNECT_PLAYER;
	SendPacket(&p);

	if (m_socket) {
		closesocket(m_socket);
		WSACleanup();
	}
#endif
}

void ClientSocket::SendPacket(void* packet)
{
	char buff[BUF_SIZE];
	memcpy(buff, packet, sizeof(packet));

	uint32 sendBytes = send(m_socket, buff, buff[0], 0);
	UE_LOG(LogTemp, Log, TEXT("Send Packet SIZE : %d"), sendBytes);
}

uint32 ClientSocket::Run()
{
	m_bStopThread = false;
	while (!m_bStopThread) {
		char buff[BUF_SIZE];
		uint32 recvBytes = recv(m_socket, buff, BUF_SIZE, 0);
		if (recvBytes != SOCKET_ERROR) {
			PacketManager::GetInstance()->ProcessPacket(buff, recvBytes);
			UE_LOG(LogTemp, Log, TEXT("Recved %d Bytes"), recvBytes);
		}
	}
	return 0;
}

void ClientSocket::Exit()
{
	CleanUp();
}

void ClientSocket::Stop()
{
	m_bStopThread = true;
}