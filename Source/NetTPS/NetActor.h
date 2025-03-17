// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NetActor.generated.h"

UCLASS()
class NETTPS_API ANetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	// Owner 검출 영역
	float findDistance = 200;
	void FindOwner();

	// 회전 값 동기화 변수
	//UPROPERTY(Replicated) // Relicated 했을 때 서버에서만 변경해야만 동기화 된다.
	UPROPERTY(ReplicatedUsing=OnRep_RotYaw)
	float rotYaw = 0;
	void Rotate();
	UFUNCTION()
	void OnRep_RotYaw();

	// 색상 변경
	UPROPERTY()
	class UMaterialInstanceDynamic* mat;
	UPROPERTY(ReplicatedUsing=OnRep_ChangeColor)
	FLinearColor matColor;
	UFUNCTION()
	void OnRep_ChangeColor();

	UFUNCTION()
	void ChangeColor();
	
};



