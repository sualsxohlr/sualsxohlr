// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerController.h"
#include "../../../Common/protocol.h"
#include "../../../Common/TSingleton.h"

class AMyPlayerController;

/**
 * 
 */
class OHLRXSUALS_API PacketManager : public TSingleton<PacketManager>
{
public:
	void Init();

	void SetPlayerController(AMyPlayerController* playerController);

	void ProcessPacket(char* packet, size_t length);

private:
	//TMap<PacketType, FD_HandlerDelegate> m_handlers;

private:	
	char m_remain_data[BUF_SIZE];
	int m_prev_data_size;

	AMyPlayerController* m_playerController;

};
