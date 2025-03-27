// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NetGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API ANetGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	void SortByPlayerId();

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameUI> gameUIFactory;

	UPROPERTY()
	class UGameUI* gameUI;
};
