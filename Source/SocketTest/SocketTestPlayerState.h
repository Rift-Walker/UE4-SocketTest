#pragma once

#include "GameFramework/PlayerState.h"
#include "Engine/EngineTypes.h"
#include "SocketTestPlayerState.generated.h"

UCLASS()
class ASocketTestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(ReplicatedUsing = OnRep_IPAddress)
	uint32 IPAddress = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UDPPort)
	int32 UDPPort = 0;

	UFUNCTION(Server, Reliable)
	void ServerAdvertisePort(int32 Port);

protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UFUNCTION()
	void OnRep_IPAddress();

	UFUNCTION()
	void OnRep_UDPPort();

	void ConnectIfNotMe();

	void ServerAdvertisePort_Implementation(int32 Port);
};
