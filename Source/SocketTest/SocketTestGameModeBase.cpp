// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "SocketTestGameModeBase.h"
#include "SocketTestGameState.h"
#include "SocketTestPlayerController.h"
#include "SocketTestPlayerState.h"

ASocketTestGameModeBase::ASocketTestGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = ASocketTestGameState::StaticClass();
	PlayerStateClass = ASocketTestPlayerState::StaticClass();
	PlayerControllerClass = ASocketTestPlayerController::StaticClass();
}

void ASocketTestGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer && NewPlayer->NetConnection)
	{
		ASocketTestPlayerState* NewPlayerState = NewPlayer->GetPlayerState<ASocketTestPlayerState>();
		if (ensure(NewPlayerState))
		{
			TSharedPtr<const FInternetAddr> RemoteAddr = NewPlayer->NetConnection->GetRemoteAddr();
			if (ensure(RemoteAddr))
			{
				RemoteAddr->GetIp(NewPlayerState->IPAddress);
			}
		}
	}
}
