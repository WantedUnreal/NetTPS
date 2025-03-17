// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compPivot = CreateDefaultSubobject<USceneComponent>(TEXT("PIVOT"));
	SetRootComponent(compPivot);
	
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	compMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	// FTimerHandle handle;
	// GetWorld()->GetTimerManager().SetTimer(handle, this, &ADoorActor::OpenClose, 1, true);
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::OpenClose()
{
	isOpen = !isOpen;
	
	FRotator rot = GetActorRotation();
	if (isOpen)
	{
		rot.Yaw = 90;
	}
	else
	{
		rot.Yaw = 0;
	}
	SetActorRotation(rot);
}

void ADoorActor::FAction(AActor* actor)
{
	OpenClose();
}
