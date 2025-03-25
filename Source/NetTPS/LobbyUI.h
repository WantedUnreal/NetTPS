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
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_RoomName;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Create;

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void OnValueChanged(float Value);
};








