// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketTestGameModeBase.generated.h"

/**
 *  A Socket test
 */
UCLASS(BlueprintType)
class SOCKETTEST_API ASocketTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;

public:

	ASocketTestGameModeBase(const FObjectInitializer& ObjectInitializer);
};
