// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSocket.h"
#ifdef USE_FSOCKET
#include "Sockets.h"
#include "SocketSubSystem.h"
#include "Common/TcpSocketBuilder.h"
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

ClientSocket::ClientSocket()
{
	if (Connect()) {
		UE_LOG(LogTemp, Error, TEXT("Connected!"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Socket Connect Failed"));
		CleanUp();
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

	int ret = connect(m_socket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
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
	closesocket(m_socket);
	WSACleanup();
#endif
}

void ClientSocket::SendPacket(void* packet)
{
	EXP_OVER* s = new EXP_OVER{ reinterpret_cast<char*>(packet) };
	WSASend(m_socket, &s->wsabuf, 1, 0, 0, &s->over, 0);
}