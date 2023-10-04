#include "pch.h"
#include "Session.h"
#include "SessionManager.h"

void Session::ProcessConnect(int id, SOCKET socket)
{
	cout << "New Client [" << id << "] Connected!" << endl;
	m_id = id;
	m_socket = socket;
	SessionManager::GetInstance()->AddClient(shared_from_this());
	RegisterRecv();
}

void Session::Disconnect()
{
	closesocket(m_socket);
}

void Session::RegisterRecv()
{
	DWORD recv_flag = 0;
	memset(&m_over.over, 0, sizeof(m_over.over));
	m_over.wsabuf.len = BUF_SIZE - m_prev_remain;
	m_over.wsabuf.buf = m_over.sendbuf + m_prev_remain;
	WSARecv(m_socket, &m_over.wsabuf, 1, 0, &recv_flag,	&m_over.over, 0);
}

void Session::SendPacket(void* packet)
{
	EXP_OVER* s = new EXP_OVER{ reinterpret_cast<char*>(packet) };
	WSASend(m_socket, &s->wsabuf, 1, 0, 0, &s->over, 0);
}