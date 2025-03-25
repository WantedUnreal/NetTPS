// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

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
		// 세션 검색 성공시 호출되는 함수 등록
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);
		// 세션 참여 성공시 호출되는 함수 등록
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnJoinSessionComplete);
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
	// 커스텀 정보 (세션 이름)
	sessionSettings.Set(FName(TEXT("DP_NAME")), diaplayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionSettings.Set(FName(TEXT("TEAM")), FString(TEXT("Wanted")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 세션 생성
	sessionInterface->CreateSession(0, FName(diaplayName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName sessionName,
	bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 성공"), *sessionName.ToString());
		// 서버가 멀티플레이 하는 Map 으로 이동!
		GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 세션 생성 실패"), *sessionName.ToString());		
	}
}

void UNetGameInstance::FindOtherSession()
{
	UE_LOG(LogTemp, Warning, TEXT("세션 검색 시작1"));
	
	// sessionSearch 만들자.
	sessionSearch = MakeShared<FOnlineSessionSearch>();

	// Lan 사용 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("서브시스템 이름 : %s"), *subsysName.ToString());
	sessionSearch->bIsLanQuery = subsysName.IsEqual(FName(TEXT("NULL")));

	// 어떤 옵션을 기준으로 검색
	sessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	sessionSearch->QuerySettings.Set(FName(TEXT("TEAM")), FString(TEXT("Wanted")), EOnlineComparisonOp::Equals);

	// 위 설정을 가지고 세션 검색
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 검색 성공!"));
		// 검색된 세션 결과들
		auto results = sessionSearch->SearchResults;
		for (int32 i = 0; i < results.Num(); i++)
		{
			FString displayName;
			results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);
			UE_LOG(LogTemp, Warning, TEXT("세션 - %d, 이름 : %s"), i, *displayName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세션 검색 실패"));
	}
}

void UNetGameInstance::JoinOtherSession()
{
	auto results = sessionSearch->SearchResults;
	if(results.Num() == 0) return;

	// 세션 이름 가져오자.
	FString displayName;
	// 5.5 이슈
	// SessionSettings 의 bUseLobbiesIfAvailable 와 bUsesPresence 의 값이 false 로 되어있다.
	results[0].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	results[0].Session.SessionSettings.bUsesPresence = true;


	results[0].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), displayName);

	// 세션 참여
	sessionInterface->JoinSession(0, FName(displayName), results[0]);
}

void UNetGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	// 만약에 참여 성공했다면
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		// 서버가 만들어 놓은 세션 URL 얻어 오자.
		FString url;
		sessionInterface->GetResolvedConnectString(sessionName, url);
		// 서버가 있는 맵으로 이동
		APlayerController* pc = GetWorld()->GetFirstPlayerController();
		pc->ClientTravel(url, TRAVEL_Absolute);
	}
}





