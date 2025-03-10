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
	class UImage* crosshair;
	
	// 총알 위젯이 추가될 패널
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* bulletPanel;

	// 총알 위젯 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> bulletUIFactory;
	

public:
	virtual void NativeConstruct() override;
	
	void ShowCrosshair(bool bShow);

	// 총알 추가
	void AddBullet();

	// 총알 하나 삭제
	void PopBullet(int32 index);
	// 총알 모두 삭제
	void PopBulletAll();
};

