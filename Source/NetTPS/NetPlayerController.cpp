// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"

#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"

void ANetPlayerController::ServerRPC_ChangeToSpectator_Implementation()
{
	// 현재 Possess 하고 있는 Pawn
	APawn* pawn = GetPawn();
	// UnPossess 하자.
	UnPossess();

	// 관전자 Pawn 만들자.
	AGameModeBase*  gm = GetWorld()->GetAuthGameMode();
	ASpectatorPawn* spectator = GetWorld()->SpawnActor<ASpectatorPawn>(gm->SpectatorClass, pawn->GetTransform());
	// 만들어진 관전자에게 Possess
	Possess(spectator);
	// 기존에 있던 Pawn 삭제
	pawn->Destroy();	
	// 5초뒤에 살아나자.
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ANetPlayerController::RespawnPlayer, 5, false);
}


void ANetPlayerController::RespawnPlayer()
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

