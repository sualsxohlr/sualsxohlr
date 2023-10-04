// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../Common/protocol.h"

#ifndef USE_FSOCKET

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND, OP_DB_LOGIN_WITH_INFO, OP_DB_LOGIN_NO_INFO, OP_DB_UPDATE, OP_PLAYER_HEAL, OP_RESPAWN_NPC, OP_NPC_AI };

class EXP_OVER
{
public:
	WSAOVERLAPPED over;
	WSABUF wsabuf;
	char sendbuf[BUF_SIZE];
	COMP_TYPE op_type;

	EXP_OVER()
	{
		wsabuf.len = BUF_SIZE;
		wsabuf.buf = sendbuf;
		op_type = OP_RECV;
		ZeroMemory(&over, sizeof(over));
	}

	EXP_OVER(char* packet)
	{
		wsabuf.len = packet[0];
		wsabuf.buf = sendbuf;
		ZeroMemory(&over, sizeof(over));
		op_type = OP_SEND;
		memcpy(sendbuf, packet, packet[0]);
	}
};
#endif

/**
 * 
 */

class OHLRXSUALS_API ClientSocket
{
public:
	ClientSocket();

	bool Connect();
	void CleanUp();

	void SendPacket(void* packet);

private:
#ifdef USE_FSOCKET
	FSocket* m_socket;
#else
	SOCKET m_socket;
	EXP_OVER m_over;
	int m_prev_remain;
#endif
};
