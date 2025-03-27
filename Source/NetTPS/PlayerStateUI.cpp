// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateUI.h"
#include "NetPlayerState.h"
#include "Components/TextBlock.h"

void UPlayerStateUI::Init(APlayerState* ps)
{
	playerState = Cast<ANetPlayerState>(ps);

	// playerState 에서 이름 변경될 때 호출되는 함수 등록
	playerState->onUpdateName.BindUObject(this, &UPlayerStateUI::UpdateName);
	// playerState 에서 점수 변경될 때 호출되는 함수 등록
	playerState->onUpdateScore.BindUObject(this, &UPlayerStateUI::UpdateScroe);
}

void UPlayerStateUI::UpdateName(FString name)
{
	Text_Name->SetText(FText::FromString(name));
}

void UPlayerStateUI::UpdateScroe(int32 score)
{
	Text_Score->SetText(FText::AsNumber(score));
}
