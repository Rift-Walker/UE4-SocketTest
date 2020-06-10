#pragma once

#include "GameFramework/PlayerController.h"
#include "Networking/Public/Common/UdpSocketReceiver.h"
#include "SocketTestPlayerController.generated.h"

#define DEFAULT_UDP_PORT 8888

class ASocketTestPlayerState;
class FSocket;
struct FIPv4Endpoint;

UCLASS()
class ASocketTestPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void OnRep_PlayerState() override;

	/** Opens a UDP port for connections and advertises it */
	void ListenUDP();

	FSocket* UdpSocket;

	FUdpSocketReceiver* SocketReceiver;

	/** UDP Message handler */
	void OnMessageReceived(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint);

	/** Sends a UDP test message */
	void SendUDP(uint32 Ip, int32 Port);

	/** Cleans up */
	void EndPlay(const EEndPlayReason::Type EndPlayReason);


public:

	/** Notifies this controller that another player is ready for UDP connections. */
	void OnPlayerReadyForConnections(const ASocketTestPlayerState* Player);
};
