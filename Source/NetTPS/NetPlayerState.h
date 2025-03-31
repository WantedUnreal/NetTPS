// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NetPlayerState.generated.h"

/**
 * 
 */

 DECLARE_DELEGATE_OneParam(FUpdateName, FString);
 DECLARE_DELEGATE_OneParam(FUpdateScore, int32);


UCLASS()
class NETTPS_API ANetPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	virtual void OnRep_Score() override;

	virtual void OnRep_PlayerName() override;

	virtual void OnRep_PlayerId() override;

	UFUNCTION()
	void OnPlayerStatePawnSet(APlayerState* Player, APawn* NewPawn, APawn* OldPawn);

	// 채팅 전송, 받기
	UFUNCTION(Server, Reliable)
	void ServerRPC_SendChat(const FString& chat);
	UFUNCTION(NetMulticast, Reliable)
	void MuliticastRPC_SendChat(const FString& chat);

public:
	FUpdateName onUpdateName;
	FUpdateScore onUpdateScore;
};
