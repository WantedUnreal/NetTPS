// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetTPSGameMode.generated.h"

UCLASS(minimalapi)
class ANetTPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANetTPSGameMode();

public:
	// 몇번째 자리에 놔야하니?
	int32 posIdx = 0;
	// 플레이어가 들어왔을 때 호출하는 함수
	void AddPlayer(class ANetTPSCharacter* player);
};



