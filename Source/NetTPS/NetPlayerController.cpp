// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"

#include "GameFramework/GameModeBase.h"

void ANetPlayerController::ServerRPC_RespawnPlayer_Implementation()
{
	// 현재 Possess 하고 있는 Pawn
	APawn* pawn = GetPawn();
	// UnPossess 하자.
	UnPossess();
	// 기존에 있던 Pawn 삭제
	pawn->Destroy();
	// 다시 Player 생성
	AGameModeBase*  gm = GetWorld()->GetAuthGameMode();
	gm->RestartPlayer(this);
}
