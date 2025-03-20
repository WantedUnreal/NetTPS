// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "HealthBar.h"
#include "NetPlayerController.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 처음에 crosshair 보이지 않게
	ShowCrosshair(false);
	// 처음에 다시하기 버튼 보이지 않게
	ShowBtnRetry(false);

	// HealthBar 위젯 찾아오자.
	hpBar = Cast<UHealthBar>(GetWidgetFromName(TEXT("HealthBar")));

	// 다시하기 버튼 이벤트 등록
	btnRetry->OnClicked.AddDynamic(this, &UMainUI::OnRetry);
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 만약에 damageUIOpacity 값이 0보다 크면
	if (damageUIOpacity > 0)
	{
		// damageUIOpactiy 를 줄이고 damageUI 의 설정
		damageUIOpacity -= InDeltaTime * 3;
		damageUIOpacity = FMath::Clamp(damageUIOpacity, 0, 1);
		damageUI->SetRenderOpacity(damageUIOpacity);
	}
}

void UMainUI::ShowCrosshair(bool bShow)
{
	// 만약에 bShow 가 true 보이게, 그렇지 않으면 보이지 않게
	ESlateVisibility e = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	crosshair->SetVisibility(e);
}

void UMainUI::ShowBtnRetry(bool bShow)
{
	// 만약에 bShow 가 true 보이게, 그렇지 않으면 보이지 않게
	ESlateVisibility e = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	btnRetry->SetVisibility(e);
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

void UMainUI::PlayDamagerUI()
{
	damageUIOpacity = 1;
}

void UMainUI::OnRetry()
{
	// Player 를 다시 시작하는 기능 : GameMode
	// UnPossess, Possess 기능 : PlayerController
	ANetPlayerController* pc = Cast<ANetPlayerController>(GetWorld()->GetFirstPlayerController());
	pc->ServerRPC_ChangeToSpectator();
	pc->SetShowMouseCursor(false);

	// 나를 삭제하자.
	RemoveFromParent();
}
