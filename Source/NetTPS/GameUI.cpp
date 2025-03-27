// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI.h"

#include "PlayerStateUI.h"
#include "NetPlayerState.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

class FPlayerStateSort
{
public:
	bool operator() (const UPlayerStateUI& a, const UPlayerStateUI& b) const
	{
		// Sort 할 때 자리 바꾸는 조건
		return a.playerState->GetPlayerId() < b.playerState->GetPlayerId(); // 오름차순
	}
};


void UGameUI::AddPlayerStateUI(APlayerState* ps)
{
	// state ui 만들자.
	UPlayerStateUI* state = CreateWidget<UPlayerStateUI>(GetWorld(), playerStateUIFactory);
	// 만들어진 state ui 에 ps 전달
	state->Init(ps);
	
	// vertical box 에 자식으로 추가
	auto s = Vertical_StateList->AddChildToVerticalBox(state);
	Alignment(s);

	// state 를 allPlayerStateUI 에 추가
	allPlayerStateUI.Add(state);
}

void UGameUI::Sort()
{
	allPlayerStateUI.Sort(FPlayerStateSort());

	// Vertical_StateList 의 자식들 다 제거
	Vertical_StateList->ClearChildren();
	// 다시 allPlayerStateUI 에 있는 순서대로 추가
	for (int32 i = 0; i < allPlayerStateUI.Num(); i++)
	{
		auto s = Vertical_StateList->AddChildToVerticalBox(allPlayerStateUI[i]);
		Alignment(s);
	}
}

void UGameUI::Alignment(UVerticalBoxSlot* s)
{
	s->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);
	s->SetPadding(FMargin(0, 0, 20, 0));
}
