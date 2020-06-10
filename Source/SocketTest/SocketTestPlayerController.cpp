#include "Engine/EngineTypes.h"
#include "Networking/Public/Common/UdpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "SocketTest.h"
#include "SocketTestGameState.h"
#include "SocketTestPlayerController.h"
#include "SocketTestPlayerState.h"

void ASocketTestPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (GetLocalRole() != ROLE_Authority)
	{
		ListenUDP();
	}
}

void ASocketTestPlayerController::ListenUDP()
{
	if (ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
	{
		UdpSocket = FUdpSocketBuilder("SocketTestListen")
			.AsNonBlocking()
			.Build();

		TSharedRef<FInternetAddr> LocalAddress = SocketSubsystem->GetLocalBindAddr(*GLog);
		LocalAddress->SetPort(DEFAULT_UDP_PORT);
		int32 BoundPort = SocketSubsystem->BindNextPort(UdpSocket, *LocalAddress, 1000, 1);

		SocketReceiver = new FUdpSocketReceiver(UdpSocket, FTimespan::FromMilliseconds(100), TEXT("UDPListenerThread"));
		SocketReceiver->OnDataReceived().BindUObject(this, &ASocketTestPlayerController::OnMessageReceived);
		SocketReceiver->Start();

		// UE_LOG(LogSocketTest, Display, TEXT("Player %s' Listening: %s:%d."), *GetPlayerName(), *LocalAddress->ToString(false), BoundPort);

		ASocketTestPlayerState* PS = GetPlayerState<ASocketTestPlayerState>();
		if (ensure(PS))
		{
			PS->ServerAdvertisePort(BoundPort);
		}
	}
}

void ASocketTestPlayerController::OnPlayerReadyForConnections(const ASocketTestPlayerState* ReadyPlayer)
{
	if (ReadyPlayer->PlayerId != PlayerState->PlayerId)
	{
		UE_LOG(LogSocketTest, Display, TEXT("Player Ready for connections: %s, local player: %s"), *ReadyPlayer->GetName(), *PlayerState->GetName());
		SendUDP(ReadyPlayer->IPAddress, ReadyPlayer->UDPPort);
	}
}

void ASocketTestPlayerController::OnMessageReceived(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint)
{
	UE_LOG(LogSocketTest, Display, TEXT("Message Received from %s"), *Endpoint.ToString());
}

void ASocketTestPlayerController::SendUDP(uint32 Ip, int32 Port)
{
	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (ensure(SocketSubsystem && UdpSocket))
	{
		const uint8 Data = 100;
		int32 BytesSent;
		TSharedRef<FInternetAddr> SendAddr = SocketSubsystem->CreateInternetAddr();
		// TSharedRefs must always be valid
		SendAddr->SetIp(Ip);
		SendAddr->SetPort(Port);
		UdpSocket->SendTo(&Data, 1, BytesSent, *SendAddr);
	}
}

void ASocketTestPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (SocketReceiver)
	{
		delete SocketReceiver;
	}

	if (ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM))
	{
		if (UdpSocket)
		{
			SocketSubsystem->DestroySocket(UdpSocket);
		}
	}

	Super::EndPlay(EndPlayReason);
}
