// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateUI.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UPlayerStateUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Name;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Score;
	UPROPERTY()
	class ANetPlayerState* playerState;

public:
	void Init(class APlayerState* ps);

	void UpdateName(FString name);
	void UpdateScroe(int32 score);
};
