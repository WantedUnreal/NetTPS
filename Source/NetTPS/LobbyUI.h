// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:	
	virtual void NativeConstruct() override;

	UPROPERTY()
	class UNetGameInstance* gi;
	
public:
	
	// 위젯 스위쳐
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
		

	// 세션 메인 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoCreate;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GoFind;
	UFUNCTION()
	void OnClickGoCreateRoom();
	UFUNCTION()
	void OnClickGoFindRoom();


	// 세션 생성 관련
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_RoomName;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Create;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromCreate;

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void OnValueChanged(float Value);

	// 세션 검색 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Find;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_BackFromFind;

	UFUNCTION()
	void OnClickFind();
	UFUNCTION()
	void OnClickBack();
};








