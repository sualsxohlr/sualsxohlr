// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../Common/protocol.h"
#include "HAL/Runnable.h"

#ifndef USE_FSOCKET
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND};
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

class OHLRXSUALS_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	bool Connect();
	void CleanUp();

	void SendPacket(void* packet);

private:
#ifdef USE_FSOCKET
	FSocket* m_socket;
#else
	SOCKET m_socket;
	EXP_OVER m_over;

#endif
	FRunnableThread* m_thread = nullptr;
	bool m_bStopThread = false;
};
