// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetTPSGameMode.h"
#include "NetTPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANetTPSGameMode::ANetTPSGameMode()
{
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
	
}

void ANetTPSGameMode::AddPlayer(class ANetTPSCharacter* player)
{
	// Player 를 추가하자.
	allPlayers.Add(player);
	// 만약에 Player 가 IsLocallyControlled 가 true => 서버캐릭터라면
	if (player->IsLocallyControlled())
	{
		ChangeTurn();
	}
	
	// 분할 갯수
	int32 positionCnt = 4;
	// 360 도 기준 각도를 positionCnt 분할 값
	float angle = 360 / positionCnt;

	// 각도를 radian 값으로 변경
	float radian = FMath::DegreesToRadians(posIdx * angle);

	// 위치 구하자.
	FVector dir = FVector( FMath::Sin(radian), FMath::Cos(radian), 0);
	FVector pos =  player->GetActorLocation() + dir * 400;
	// 구한 위치로 Player 이동
	player->SetActorLocation(pos);
	//posIdx 를 증가
	posIdx++;
}

void ANetTPSGameMode::ChangeTurn()
{
	// 만약에 currTurnIdx 가 -1 이 아니면
	if (currTurnIdx != -1)
	{
		// 이전 Turn 인 사람 canMakeCube 를 false 로 하자.
		allPlayers[currTurnIdx]->canMakeCube = false;		
	}
	
	// 다음 턴으로 만들자.
	currTurnIdx++;
	if (currTurnIdx >= allPlayers.Num())
	{
		currTurnIdx = 0;
	}
	//currTurnIdx = (currTurnIdx + 1) % allPlayers.Num();
	
	// currTurnIdx 의 Player 를 현재 Turn 으로 설정
	allPlayers[currTurnIdx]->canMakeCube = true;
}
