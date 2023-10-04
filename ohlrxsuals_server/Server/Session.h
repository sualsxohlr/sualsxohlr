#pragma once
#include "expover.h"

class Session : public enable_shared_from_this<Session>
{
public:
	void ProcessConnect(int id, SOCKET socket);
	void Disconnect();
	
	void SendPacket(void* packet);
	void RegisterRecv();

public:
	int m_id;
	string m_name;

	SOCKET m_socket;
	EXP_OVER m_over;
	int m_prev_remain;
};

