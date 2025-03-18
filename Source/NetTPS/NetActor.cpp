// Fill out your copyright notice in the Description page of Project Settings.


#include "NetActor.h"

#include "NetTPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ANetActor::ANetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	SetRootComponent(compMesh);

	// 동기화가 되기 위한 필수 조건
	bReplicates = true;
}

// Called when the game starts or when spawned
void ANetActor::BeginPlay()
{
	Super::BeginPlay();

	// Dynamic 매터리얼 설정
	mat = compMesh->CreateDynamicMaterialInstance(0);

	// 1초마다 색변경 하는 타이머
	FTimerHandle colorHandle;
	GetWorldTimerManager().SetTimer(colorHandle, this, &ANetActor::ChangeColor, 1.0f, true);

	// 1초마다 크기변경 하는 타이머
	FTimerHandle scaleHandle;
	GetWorldTimerManager().SetTimer(scaleHandle, this, &ANetActor::ChangeScale, 1.0f, true);

	// 1초마다 위치변경 하는 타이머
	FTimerHandle locationHandle;
	GetWorldTimerManager().SetTimer(locationHandle, this, &ANetActor::ChangeLocation, 1.0f, true);
}

void ANetActor::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Relplicated 할 변수들 등록
	DOREPLIFETIME(ANetActor, rotYaw);
	DOREPLIFETIME(ANetActor, matColor);
}

// Called every frame
void ANetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString ownerStr = GetOwner() != nullptr ? GetOwner()->GetActorNameOrLabel() : TEXT("No Owner");
	DrawDebugString(GetWorld(), GetActorLocation(), ownerStr, nullptr, FColor::Yellow, 0, true);

	FindOwner();
	Rotate();
}

void ANetActor::FindOwner()
{
	// SetOwner 는 서버에서만 동작하게 해야함.
	//SetOwner()

	// 만약에 서버가 아니라면 함수를 나가자.
	if (HasAuthority() == false) return;

	// 모든 Player 를 찾자.
	TArray<AActor*> allPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetTPSCharacter::StaticClass(), allPlayer);
	// findDistance 범위에 있고 그중에서 가장 가까운 Player 를 뽑자.
	AActor* newOwner = nullptr;
	float closestDist = findDistance;
	for (int32 i = 0; i < allPlayer.Num(); i++)
	{
		float dist = FVector::Distance(GetActorLocation(), allPlayer[i]->GetActorLocation());
		if (dist < closestDist)
		{
			closestDist = dist;
			newOwner = allPlayer[i];
		}
	}	
	// 현재 Owner 랑  검색한 Owner 가 다르면
	if (GetOwner() != newOwner)
	{
		// 나의 Owner 를 검색한 Owner 로 설정
		SetOwner(newOwner);
	}
}

void ANetActor::Rotate()
{
	// 만약에 서버라면
	if (HasAuthority())
	{
		// yaw 축으로 회전시키자.
		AddActorLocalRotation(FRotator(0, 50 * GetWorld()->DeltaTimeSeconds, 0));

		// 현재 Yaw 값을  rotYaw 에 설정
		rotYaw = GetActorRotation().Yaw;
	}
}

// 클라이언트에서만 호출된다!!
void ANetActor::OnRep_RotYaw()
{
	FRotator rot = GetActorRotation();
	rot.Yaw = rotYaw;
	SetActorRotation(rot);
}

void ANetActor::OnRep_ChangeColor()
{
	mat->SetVectorParameterValue(TEXT("FloorColor"), matColor);
}

void ANetActor::ChangeColor()
{
	// 만약에 서버가 아니라면 함수를 나가자.
	if (HasAuthority() == false) return;

	// 색상 랜덤 컬러 설정
	matColor = FLinearColor::MakeRandomColor();

	OnRep_ChangeColor();
}

void ANetActor::ChangeScale()
{
	// 만약에 Owner 가 나의 Pawn 으로 되어있다면
	if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
	//if (!HasAuthority())
	{
		// 서버에게 크기 변경 요청
		ServerRPC_ChangeScale();
	}
}

void ANetActor::ServerRPC_ChangeScale_Implementation()
{
	// 랜덤한 크기값을 뽑자.
	float rand = FMath::RandRange(0.5f, 2.0f);
	// 뽑은 크기값을을 모든 클라이언트에게 보내자.
	MulitcastRPC_ChangeScale(FVector(rand));
}

void ANetActor::MulitcastRPC_ChangeScale_Implementation(FVector scale)
{
	SetActorScale3D(scale);
}

void ANetActor::ChangeLocation()
{
	// 만약에 서버가 아니면 함수 나가자.
	if (HasAuthority() == false) return;

	// 랜텀 위치 
	FVector rand = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(100));
	// 뽑은 위치 값을 모든 클라이언트에게 보내자.
	MulitcastRPC_ChangeLocation(rand);
}

void ANetActor::MulitcastRPC_ChangeLocation_Implementation(FVector location)
{
	SetActorLocation(location);
}






