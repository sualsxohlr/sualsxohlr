// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Common/protocol.h"
#include "../../../Common/TSingleton.h"

DECLARE_DELEGATE_OneParam(FD_HandlerDelegate, char*);

/**
 * 
 */
class OHLRXSUALS_API PacketManager : public TSingleton<PacketManager>
{
public:
	void Init();
	
	void ProcessPacket(char* packet, size_t length);
	
	void SCLoginInfoHandler(char* packet);
	void SCMovePlayerHandler(char* packet);

private:
	FD_HandlerDelegate SCLoginDelegate;
	FD_HandlerDelegate SCMovePlayerDelegate;

	TMap<PacketType, FD_HandlerDelegate> m_handlers;

private:	
	char m_remain_data[BUF_SIZE];
	int m_prev_data_size;

};
