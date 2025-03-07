// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerAnimInstance.h"

#include "NetTPSCharacter.h"

void UNetPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// ANetTPSCharacter 가져오자.
	player = Cast<ANetTPSCharacter>(TryGetPawnOwner());
}

void UNetPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player)
	{
		// 총 들고 있는지 여부 설정
		bHasPistol = player->bHasPistol;

		// 좌 / 우 에 따른 애니메이션 값
		dirH = FVector::DotProduct(player->GetVelocity(), player->GetActorRightVector());
		// 앞 / 뒤 에 따른 애니메이션 값
		dirV = FVector::DotProduct(player->GetVelocity(), player->GetActorForwardVector());
	}
}
