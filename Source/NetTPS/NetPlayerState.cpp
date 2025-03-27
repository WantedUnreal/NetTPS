// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerState.h"
#include "NetGameState.h"

void ANetPlayerState::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ANetPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ANetPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// OnPawnSet 함수 등록
	OnPawnSet.AddDynamic(this, &ANetPlayerState::OnPlayerStatePawnSet);
}

// 서버에서 호출 안됨
void ANetPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	// PlayerStateUI 에 있는 점수 변경 (딜리게이트 사용)
	onUpdateScore.ExecuteIfBound(GetScore());
}

// 서버에서 호출 됨
void ANetPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();

	// PlayerStateUI 에 있는 이름 변경 (딜리게이트 사용)
	onUpdateName.ExecuteIfBound(GetPlayerName());
}

// 서버에서 호출 안됨
void ANetPlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();

	// ANetGameState 가져오자
	ANetGameState* gs = Cast<ANetGameState>(GetWorld()->GetGameState());
	// 가져온 ANetGameState 의 SortByPlayerId 실행
	gs->SortByPlayerId();
}

void ANetPlayerState::OnPlayerStatePawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	if (NewPawn != nullptr)
	{
		// PlayerState 의 이름, 점수, 아이디, Pawn 모두 셋팅되어 있다.
	}
}
