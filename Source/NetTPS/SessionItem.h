// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionItem.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API USessionItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_RoomInfo;

	int32 roomIdx;

	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClickJoin();

	void SetInfo(int32 idx, FString info);
};
