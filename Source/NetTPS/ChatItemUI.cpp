// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatItemUI.h"

#include "Components/TextBlock.h"

void UChatItemUI::SetContent(FString chat)
{
	Text_Content->SetText(FText::FromString(chat));
}
