// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientSocket.h"
#include "../../../Common/protocol.h"
#include "MyPlayerController.generated.h"

DECLARE_DELEGATE_OneParam(FD_HandlerDelegate, char*);

/**
 * 
 */
UCLASS()
class OHLRXSUALS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void EnterPlayer(uint32 id, FVector location, FRotator rotator);

	void SCLoginInfoHandler(char* packet);
	void SCMovePlayerHandler(char* packet);

public:
	FD_HandlerDelegate SCLoginDelegate;
	FD_HandlerDelegate SCMovePlayerDelegate;

public:
	ClientSocket* m_socket;
};
