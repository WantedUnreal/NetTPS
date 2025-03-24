// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void UNetGameInstance::Init()
{
	Super::Init();

	// 현재 사용하는 서브시스템을 가져오자.
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();

		// 세션 생성 성공시 호출되는 함수 등록
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
	}
}

void UNetGameInstance::CreateMySession(FString diaplayName, int32 playerCount)
{
	// 세션을 만들기 위한 옵션 설정
	FOnlineSessionSettings sessionSettings;

	// Lan 사용 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름 : %s"), *subsysName.ToString());
	sessionSettings.bIsLANMatch = subsysName.IsEqual(FName(TEXT("NULL")));

	// Lobby 를 사용할지 여부
	sessionSettings.bUseLobbiesIfAvailable = true;
	// 친구 상태를 확인할 수 있는 여부
	sessionSettings.bUsesPresence = true;
	// 세션 검색을 허용할지 여부
	sessionSettings.bShouldAdvertise = true;
	// 세션 최대 인원 설정
	sessionSettings.NumPublicConnections = playerCount;

	// 세션 생성
	sessionInterface->CreateSession(0, FName(diaplayName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName sessionName,
	bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *sessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 실패"), *sessionName.ToString());		
	}
}
