// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 처음에 crosshair 보이지 않게
	ShowCrosshair(false);
}

void UMainUI::ShowCrosshair(bool bShow)
{
	// 만약에 bShow 가 true 보이게, 그렇지 않으면 보이지 않게
	ESlateVisibility e = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	crosshair->SetVisibility(e);
}

void UMainUI::AddBullet()
{
	// 총알 위젯 만들자.
	UUserWidget* bullet = CreateWidget(GetWorld(), bulletUIFactory);
	// 만들어진 총알 위젯을 bulletPanel 에 자식으로 붙이자.
	bulletPanel->AddChildToHorizontalBox(bullet);
}

void UMainUI::PopBullet(int32 index)
{
	// index 번째 총알 위젯 지우자
	bulletPanel->RemoveChildAt(index);
}

void UMainUI::PopBulletAll()
{
	// bulletPanel 의 자식 갯수
	int32 remainBullet = bulletPanel->GetChildrenCount();
	// 모든 총알 지워라
	for (int32 i = remainBullet - 1; i >= 0; i--)
	{
		PopBullet(i);
	}
}
