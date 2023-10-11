#pragma once
#include "../../Common/TSingleton.h"

class Session;
class SessionManager : public TSingleton<SessionManager>
{
public:
	int AddClient(const shared_ptr<Session>& session);
	shared_ptr<Session> FindClient(int id);
	void RemoveClient(int id);

	unordered_map<int, shared_ptr<Session>> GetClients() { return clients; }
	void Broadcast(void* packet);

private:
	unordered_map<int, shared_ptr<Session>> clients;
	atomic_int m_client_id = 0;
	mutex m_lock;
};

