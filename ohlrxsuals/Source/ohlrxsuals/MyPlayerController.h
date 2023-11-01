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

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

	void SetCharacter(ACharacter* character) { m_playerCharacter = character; }

	void EnterPlayer(uint32 id, FVector location, FRotator rotator);

	void OnGameExited();

	void SendPlayerInfo();

	void SCLoginInfoHandler(char* packet);
	void SCMovePlayerHandler(char* packet);

public:
	FD_HandlerDelegate SCLoginDelegate;
	FD_HandlerDelegate SCMovePlayerDelegate;

	class ACharacter* m_playerCharacter;

	ClientSocket* m_socket;
};
