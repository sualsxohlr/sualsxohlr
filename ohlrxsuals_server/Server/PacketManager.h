#pragma once
#include "../../Common/TSingleton.h"

class Session;
class PacketManager : public TSingleton<PacketManager>
{
public:
	void Init();
	void ProcessPacket(shared_ptr<Session> session, char* packet, size_t length);

private:
	unordered_map<PacketType, function<void(shared_ptr<Session>, char*)>> m_handlers;
	unsigned char m_remain_data[BUF_SIZE];
	int m_prev_data_size;

private:
	void BindHandler(PacketType, void(PacketManager::* handler)(shared_ptr<Session>, char*));
	
	void CSLoginHandler(shared_ptr<Session> session, char* packet);	
	void CSPlayerInfoHandler(shared_ptr<Session> session, char* packet);
	void CSDisconnectPlayerHandler(shared_ptr<Session> session, char* packet);
};

