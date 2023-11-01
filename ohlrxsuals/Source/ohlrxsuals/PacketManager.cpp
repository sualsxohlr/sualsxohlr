// Fill out your copyright notice in the Description page of Project Settings.

#include "PacketManager.h"
//#include "ohlrxsuals.h"

void PacketManager::Init()
{
	//m_handlers.Add(PacketType::SC_LOGIN_INFO, *&SCLoginDelegate);
	//m_handlers.Add(PacketType::SC_MOVE_PLAYER, *&SCMovePlayerDelegate);
}

void PacketManager::ProcessPacket(char* packet, size_t length)
{
	int data_to_process = static_cast<int>(length) + m_prev_data_size;
	while (data_to_process > 0) {
		int packet_size = packet[0];
		if (packet_size > data_to_process) break;

		PacketType packet_type = (PacketType)packet[1];
		//m_handlers[packet_type].ExecuteIfBound(packet);
		switch (packet_type) {
		case PacketType::SC_LOGIN_INFO:
			m_playerController->SCLoginDelegate.ExecuteIfBound(packet);
			break;
		case PacketType::SC_MOVE_PLAYER:
			m_playerController->SCMovePlayerDelegate.ExecuteIfBound(packet);
			break;
		}

		packet += packet_size;
		data_to_process -= packet_size;
	}
	m_prev_data_size = data_to_process;
	if (m_prev_data_size > 0) {
		memcpy(m_remain_data, packet, m_prev_data_size);
	}
}

void PacketManager::SetPlayerController(AMyPlayerController* playerController)
{
	if (playerController)
		m_playerController = playerController;
}