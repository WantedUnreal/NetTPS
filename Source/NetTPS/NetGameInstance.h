// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NETTPS_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// BeginPlay
	virtual void Init() override;

	// 세션 생성
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString diaplayName, int32 playerCount);
    void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	// 세션 조회
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 참여
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession();
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

public:
	// 세션의 모든 처리를 진행
	IOnlineSessionPtr sessionInterface;

	// 세션 검색할 때 사용하는 객체
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
};
