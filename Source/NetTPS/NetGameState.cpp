// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameState.h"
#include "GameUI.h"
#include "GameFramework/PlayerState.h"

void ANetGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ANetGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	//PlayerState->GetPlayerId();
	//PlayerState->GetPlayerName();
	//PlayerState->GetScore()

	// gameUI 가 없을때만
	if (gameUI == nullptr)
	{
		//GameUI 만들자
		gameUI = CreateWidget<UGameUI>(GetWorld(), gameUIFactory);
		gameUI->AddToViewport();
	}

	//GameUI PlayerStateUI 하나 추가해줘
	gameUI->AddPlayerStateUI(PlayerState);
}

void ANetGameState::SortByPlayerId()
{
	gameUI->Sort();
}
