#include "Engine.h"
#include "CoreMinimal.h"
#include "SocketTestPlayerState.h"
#include "SocketTestPlayerController.h"
#include "SocketTestGameState.h"
#include "UnrealNetwork.h"

void ASocketTestPlayerState::ServerAdvertisePort_Implementation(int32 Port)
{
	UDPPort = Port;
}

void ASocketTestPlayerState::BeginPlay()
{
	// For late join
	if (IPAddress && UDPPort)
	{
		ConnectIfNotMe();
	}
}

void ASocketTestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASocketTestPlayerState, IPAddress);
	DOREPLIFETIME(ASocketTestPlayerState, UDPPort);
}

void ASocketTestPlayerState::OnRep_IPAddress()
{
	if (IPAddress && UDPPort)
	{
		ConnectIfNotMe();
	}
}

void ASocketTestPlayerState::OnRep_UDPPort()
{
	if (IPAddress && UDPPort)
	{
		ConnectIfNotMe();
	}
}

void ASocketTestPlayerState::ConnectIfNotMe()
{
	if (ensure(GetWorld()))
	{
		for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
		{
			if (ASocketTestPlayerController* SocketTestPlayerController = Cast<ASocketTestPlayerController>(*it))
			{
				if (SocketTestPlayerController != GetOwner())
				{
					SocketTestPlayerController->OnPlayerReadyForConnections(this);
				}
			}
		}
	}
}

