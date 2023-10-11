#include "pch.h"
#include "SessionManager.h"
#include "Session.h"

int SessionManager::AddClient(const shared_ptr<Session>& session)
{
	m_lock.lock();
	int new_id = m_client_id++;
	clients[new_id] = session;
	clients[new_id]->m_id = new_id;
	m_lock.unlock();
	return new_id;
}

shared_ptr<Session> SessionManager::FindClient(int id)
{
	m_lock.lock();
	auto iter = clients.find(id);
	m_lock.unlock();
	if (iter != clients.end()) {
		return iter->second;
	}
	else {
		exit(1);
	}
}

void SessionManager::RemoveClient(int id)
{
	cout << "Client [" << id << "] 접속 종료! " << endl;
	m_lock.lock();
	clients[id]->Disconnect();
	clients.erase(id);
	m_lock.unlock();
}

void SessionManager::Broadcast(void* packet)
{
	for (auto& c : clients)
		c.second->SendPacket(packet);
}