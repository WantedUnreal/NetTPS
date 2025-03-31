// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI.h"

#include "PlayerStateUI.h"
#include "NetPlayerState.h"
#include "ChatItemUI.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
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



void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	// EditableTextBox 에서 엔터쳤을 때 호출되는 함수 등록
	Edit_Chat->OnTextCommitted.AddDynamic(this, &UGameUI::OnTextBoxCommitted);

	// Border_Empty 클릭 이벤트 함수 등록
	Border_Empty->OnMouseButtonDownEvent.BindDynamic(this, &UGameUI::OnPointerEvent);
}

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

void UGameUI::AddChat(FString chat)
{
	// 현재 스크롤 된 값
	float scrollOffset = Scroll_Chat->GetScrollOffset();
	// 스크롤이 맨 끝일때의 값
	float scrollEndofOffset = Scroll_Chat->GetScrollOffsetOfEnd();
	
	// ChatItem 을 만들어서 Scroll_Chat 에 자식으로!
	UChatItemUI* chatItem = CreateWidget<UChatItemUI>(GetWorld(), chatItemFactory);
	Scroll_Chat->AddChild(chatItem);
	chatItem->SetContent(chat);	

	// 만약에 스크롤이 맨 끝이라면
	if (scrollOffset == scrollEndofOffset)
	{
		// 개행이된 내용은 ScrollToEnd 동작을 정상적으로 하지 않는 문제때문에 0.01초 뒤에 실행되게 하자.
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			// 강제로 스크롤을 맨 끝으로 이동시키자.
			Scroll_Chat->ScrollToEnd();
		}, 0.01f, false);		
	}
}

void UGameUI::OnTextBoxCommitted(const FText& Text,
                                 ETextCommit::Type CommitMethod)
{
	// 만약에 Enter 를 쳤다면
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Text.ToString());

		// 서버에게 채팅을 전달
		// PlayerState 가져오자.
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		ANetPlayerState* ps = pc->GetPlayerState<ANetPlayerState>();
		ps->ServerRPC_SendChat(Text.ToString());
		
		// Edit_Chat 내용을 초기화
		Edit_Chat->SetText(FText());
	}
	// 만약에 Enter 를 친 후 Focus 를 잃었다면
	else if (CommitMethod == ETextCommit::Type::OnCleared)
	{
		// 강제로 Edit_Chat 에 Focus 를 하자.
		Edit_Chat->SetFocus();
	}
}

FEventReply UGameUI::OnPointerEvent(FGeometry MyGeometry,
	const FPointerEvent& MouseEvent)
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
	pc->SetShowMouseCursor(false);
	return FEventReply();
}
