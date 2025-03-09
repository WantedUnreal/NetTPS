// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	currHP = maxHP;
}

float UHealthBar::UpdateHPBar(float damage)
{
	currHP -= damage;

	float pecent = currHP / maxHP;

	hpBar->SetPercent(pecent);

	return currHP;
}
