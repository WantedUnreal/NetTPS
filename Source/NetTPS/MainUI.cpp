// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"


void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	ShowCrosshair(false);
}

void UMainUI::ShowCrosshair(bool isShow)
{
	ESlateVisibility e = isShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	img_Crosshair->SetVisibility(e);
}

void UMainUI::AddBullet()
{
	UUserWidget* bullet = CreateWidget(GetWorld(), bulletUIFactory);
	bulletPanel->AddChildToHorizontalBox(bullet);
}

void UMainUI::PopBullet(int32 index)
{
	bulletPanel->RemoveChildAt(index);
}

void UMainUI::PopBulletAll()
{
	int32 remainBullet = bulletPanel->GetChildrenCount();

	for (int32 i = remainBullet - 1; i >= 0; i--)
	{
		PopBullet(i);
	}
}
