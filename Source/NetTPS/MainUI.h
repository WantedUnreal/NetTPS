// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* img_Crosshair;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* bulletPanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> bulletUIFactory;

public:
	virtual void NativeConstruct() override;
	void ShowCrosshair(bool isShow);

	void AddBullet();
	void PopBullet(int32 index);
	void PopBulletAll();
};
