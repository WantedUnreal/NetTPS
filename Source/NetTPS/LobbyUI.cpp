// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUI.h"

#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	// GameInstance 가져오자
	gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	// create 버튼 눌렀을 때 호출되는 함수 등록
	Btn_Create->OnClicked.AddDynamic(this, &ULobbyUI::CreateSession);
	// Slider 의 값이 변경될 때마다 호출되는 함수 등록
	Slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyUI::OnValueChanged);

	Btn_GoCreate->OnClicked.AddDynamic(this, &ULobbyUI::OnClickGoCreateRoom);
	Btn_GoFind->OnClicked.AddDynamic(this, &ULobbyUI::OnClickGoFindRoom);
	Btn_Find->OnClicked.AddDynamic(this, &ULobbyUI::OnClickFind);
	Btn_BackFromCreate->OnClicked.AddDynamic(this, &ULobbyUI::OnClickBack);
	Btn_BackFromFind->OnClicked.AddDynamic(this, &ULobbyUI::OnClickBack);
}

void ULobbyUI::OnClickGoCreateRoom()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void ULobbyUI::OnClickGoFindRoom()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

void ULobbyUI::CreateSession()
{
	// 방 제목
	FString roomName = Edit_RoomName->GetText().ToString();

	// 만약에 방 제목이 없다면 함수 나가자.
	if (roomName.IsEmpty()) return;
	
	// 인원 수
	int32 playerCount = Slider_PlayerCount->GetValue();
	// 생성
	gi->CreateMySession(roomName, playerCount);
}

void ULobbyUI::OnValueChanged(float Value)
{
	// Value 값을 text 값으로 변경 후 셋팅
	Text_PlayerCount->SetText(FText::AsNumber(Value));
}

void ULobbyUI::OnClickFind()
{
	gi->FindOtherSession();
}

void ULobbyUI::OnClickBack()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}




