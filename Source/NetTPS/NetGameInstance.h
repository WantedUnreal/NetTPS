// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"

// 세션 검색 완료시 호출되는 함수 등록하는 Delegate
DECLARE_DELEGATE_TwoParams(FFindComplete, int32, FString);

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
	void JoinOtherSession(int32 sessionIdx);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	// 스팀으로 했을때 한글 깨짐현상 관련
	FString StringBase64Encode(FString str);
	FString StringBase64Decode(FString str);

public:
	// 세션의 모든 처리를 진행
	IOnlineSessionPtr sessionInterface;

	// 세션 검색할 때 사용하는 객체
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	// 세션 검색 완료시 호출되는 Delegate
	FFindComplete onFindComplete;
};
