// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionItem.h"
#include "NetGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USessionItem::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Join->OnClicked.AddDynamic(this, &USessionItem::OnClickJoin);
}

void USessionItem::OnClickJoin()
{
	UNetGameInstance* gi = Cast<UNetGameInstance>(GetWorld()->GetGameInstance());
	gi->JoinOtherSession(roomIdx);
}

void USessionItem::SetInfo(int32 idx, FString info)
{
	// 몇 번째 방인지
	roomIdx = idx;
	// 방 정보 업데이트
	Text_RoomInfo->SetText(FText::FromString(info));
}
