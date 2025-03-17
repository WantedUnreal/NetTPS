// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "DoorActor.generated.h"

UCLASS()
class NETTPS_API ADoorActor : public AActor, public IMyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	class USceneComponent* compPivot;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	bool isOpen = false;
	void OpenClose();

	virtual void FAction(AActor* actor) override;
};
