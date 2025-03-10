// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

#include "Components/ProgressBar.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	// 현재 HP 를 최대 HP 로 설정
	currHP = maxHP;
}

void UHealthBar::UpdateHPBar(float damage)
{
	// 현재 HP 를 damage 만큼 줄이자.
	currHP -= damage;
	// 현재 HP를 0 ~ 1 사이의 값으로 만들자.
	float percent = currHP / maxHP;
	// 위에 계산된 값을 hpBar 에 설정
	hpBar->SetPercent(percent);
}
