#include "pch.h"
#include "Server.h"
#include "SessionManager.h"
#include "Session.h"
#include "PacketManager.h"

void Server::Initialize()
{
	PacketManager::GetInstance()->Init();

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	m_listen_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_listen_socket == INVALID_SOCKET) {
		cout << "Error " << WSAGetLastError() << endl;
		exit(0);
	}

	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (::bind(m_listen_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR) {
		cout << "Error " << WSAGetLastError() << endl;
		exit(0);
	}

	if (listen(m_listen_socket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Error " << WSAGetLastError() << endl;
		exit(0);
	}

	SOCKADDR_IN cl_addr;
	int addr_size = sizeof(cl_addr);
	int client_id = 0;

	m_iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	if (m_iocp_handle == nullptr) {
		cout << "Error " << WSAGetLastError() << endl;
		exit(0);
	}
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_listen_socket), m_iocp_handle, 9999, 0);

	m_client_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_client_socket == INVALID_SOCKET) {
		cout << "Error " << WSAGetLastError() << endl;
		exit(0);
	}
	
	m_exp_over.op_type = OP_ACCEPT;
	int ret = AcceptEx(m_listen_socket, m_client_socket, m_exp_over.sendbuf, 0, addr_size + 16, addr_size + 16, 0, &m_exp_over.over);
	if (ret == SOCKET_ERROR) {
		cout << "Error" << WSAGetLastError() << endl;
		exit(0);
	}
}

void Server::Run()
{
	int num_threads = std::thread::hardware_concurrency();
	for (int i = 0; i < num_threads; ++i)
		m_worker_thread.emplace_back([this]() {DoWorker(); });

	for (auto& th : m_worker_thread)
		th.join();
}

void Server::CleanUp()
{
	closesocket(m_listen_socket);
	WSACleanup();
}

void Server::DoWorker()
{
	while (true) {
		DWORD num_bytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over = nullptr;
		BOOL ret = GetQueuedCompletionStatus(m_iocp_handle, &num_bytes, &key, &over, INFINITE);
		EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(over);
		int client_id = static_cast<int>(key);
		if (FALSE == ret) {
			if (exp_over->op_type == OP_ACCEPT) {
				cout << "Accept Error" << endl;
			}
			else {
				cout << "error" << endl;
				SessionManager::GetInstance()->RemoveClient(client_id);
				if (exp_over->op_type == OP_SEND) delete exp_over;
				continue;
			}
		}
		// 23.10.02 : 여기 switch문도 함수 바인딩하기

		switch (exp_over->op_type) {
		case OP_ACCEPT:
			ProcessAccept();
			break;
		case OP_RECV:
			ProcessRecv(exp_over, num_bytes, client_id);
			break;
		case OP_SEND:
			ProcessSend(exp_over);
			break;
		}
	}
}

void Server::ProcessAccept()
{
	int new_id = m_client_id++;
	shared_ptr<Session> session = make_shared<Session>();
	session->ProcessConnect(new_id, m_client_socket);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_client_socket), m_iocp_handle, new_id, 0);
	m_client_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	ZeroMemory(&m_exp_over.over, sizeof(m_exp_over.over));
	int addr_size = sizeof(SOCKADDR_IN);
	AcceptEx(m_listen_socket, m_client_socket, m_exp_over.sendbuf, 0, addr_size + 16, addr_size + 16, 0, &m_exp_over.over);
}

void Server::ProcessRecv(EXP_OVER* exp_over, ULONG_PTR num_bytes, int client_id)
{
	shared_ptr<Session> session = SessionManager::GetInstance()->FindClient(client_id);
	PacketManager::GetInstance()->ProcessPacket(session->shared_from_this(), exp_over->sendbuf, num_bytes);
	session->RegisterRecv();
}

void Server::ProcessSend(EXP_OVER* exp_over)
{
	delete exp_over;
}