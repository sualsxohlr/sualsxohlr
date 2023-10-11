// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientSocket.h"
#include "MyPlayerController.generated.h"

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

public:
	ClientSocket* m_socket;
};
