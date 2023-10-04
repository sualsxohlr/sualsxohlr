#pragma once
#include "expover.h"

class Server
{
private:
	HANDLE m_iocp_handle;
	SOCKET m_listen_socket;
	SOCKET m_client_socket;
	EXP_OVER m_exp_over;

	vector<thread> m_worker_thread;

	atomic_int m_client_id = 0;

public:
	Server() {};
	~Server() {};

	void Initialize();
	void Run();
	void CleanUp();

	void DoWorker();
	void ProcessPacket(int c_id, char* packet);

	void ProcessAccept();
	void ProcessRecv(EXP_OVER* exp_over, ULONG_PTR num_bytes, int client_id);
	void ProcessSend(EXP_OVER* exp_over);
};

