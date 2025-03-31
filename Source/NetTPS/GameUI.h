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
	UFUNCTION()
	void OnTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	FEventReply OnPointerEvent(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
	
	virtual void NativeConstruct() override;
	
	void AddPlayerStateUI(class APlayerState* ps);
	void Sort();
	void Alignment(class UVerticalBoxSlot* s);

	void AddChat(FString chat);

public:
	// 플레이어 상태 관련(이름, 점수)
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* Vertical_StateList;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerStateUI> playerStateUIFactory;
	UPROPERTY()
	TArray<class UPlayerStateUI*> allPlayerStateUI;

	// 채팅 관련
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_Chat;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_Chat;
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Empty;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChatItemUI> chatItemFactory;	
};
