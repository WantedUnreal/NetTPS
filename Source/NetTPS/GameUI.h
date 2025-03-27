// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UGameUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddPlayerStateUI(class APlayerState* ps);
	void Sort();
	void Alignment(class UVerticalBoxSlot* s);

public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* Vertical_StateList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerStateUI> playerStateUIFactory;

	UPROPERTY()
	TArray<class UPlayerStateUI*> allPlayerStateUI;
};
