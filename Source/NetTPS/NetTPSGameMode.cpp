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
