// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatItemUI.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UChatItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Content;

	void SetContent(FString chat);
};
