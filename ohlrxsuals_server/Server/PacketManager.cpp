#include "pch.h"
#include "PacketManager.h"
#include "Session.h"

void PacketManager::Init()
{
	BindHandler(PacketType::CS_LOGIN, &PacketManager::CSLoginHandler);
}

void PacketManager::BindHandler(PacketType type, void(PacketManager::* handler)(shared_ptr<Session>, char*))
{
	m_handlers[type] = [this, handler](shared_ptr<Session> session, char* packet)
	{
		(this->*handler)(session, packet);
	};
}

void PacketManager::ProcessPacket(shared_ptr<Session> session, char* packet, size_t length)
{
	int data_to_process = static_cast<int>(length) + m_prev_data_size;
	while (data_to_process > 0) {
		int packet_size = packet[0];
		if (packet_size > data_to_process) break;

		PacketType packet_type = (PacketType)packet[1];
		m_handlers[packet_type](session, packet);

		packet += packet_size;
		data_to_process -= packet_size;
	}
	m_prev_data_size = data_to_process;
	if (m_prev_data_size > 0) {
		memcpy(m_remain_data, packet, m_prev_data_size);
	}
}

void PacketManager::CSLoginHandler(shared_ptr<Session> session, char* packet)
{
	CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
	session->m_name = p->name;
	
	SC_LOGIN_INFO_PACKET loginPacket;
	loginPacket.size = sizeof(SC_LOGIN_INFO_PACKET);
	loginPacket.type = PacketType::SC_LOGIN_INFO;

	session->SendPacket(&loginPacket);
}
