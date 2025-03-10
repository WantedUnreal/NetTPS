// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetTPSCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthBar.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"
#include "Pistol.h"
#include "MainUI.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ANetTPSCharacter

ANetTPSCharacter::ANetTPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 총 관련 컴포넌트
	compGun = CreateDefaultSubobject<USceneComponent>(TEXT("GUN"));
	compGun->SetupAttachment(GetMesh(), TEXT("GunPosition"));
	compGun->SetRelativeLocation(FVector(-7.144f, 3.68f, 4.136f));
	compGun->SetRelativeRotation(FRotator(3.406493f, 75.699540f, 6.642412f));

	// HealthBar 컴포넌트
	compHP = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP"));
	compHP->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANetTPSCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ANetTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANetTPSCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANetTPSCharacter::Look);

		// 총집기 함수 등록
		EnhancedInputComponent->BindAction(TakePistolAction, ETriggerEvent::Started, this, &ANetTPSCharacter::TakePistol);

		// 총쏘기 함수 등록
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ANetTPSCharacter::Fire);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANetTPSCharacter::InitBulletUI()
{
	// 현재 총알 다 지우자.
	mainUI->PopBulletAll();
	
	// 현재 총알 갯수만큼 총알을 채우자.
	for (int32 i = 0; i < ownedPistol->currBulletCount; i++)
	{
		mainUI->AddBullet();
	}
}

void ANetTPSCharacter::Reload()
{
	// 만약에 총을 들고 있지 않으면 함수 나가자.
	if (bHasPistol == false) return;

	// 만약에 재장전 중이면 함수를 나가자.
	if (bReloading) return;

	// 만약에 총알이 최대라면 함수를 나가자.
	if (ownedPistol->currBulletCount == ownedPistol->maxBulletCount) return;

	// 재장전 중이다라고 설정
	bReloading = true;

	// 재장전 애니메이션 실행
	PlayAnimMontage(fireMontage, 1.0f, TEXT("Reload"));
}

void ANetTPSCharacter::ReloadComplete()
{
	// 재장전 끝이다 설정
	bReloading = false;
	// 총알 갯수 최대 갯수 설정
	ownedPistol->currBulletCount = ownedPistol->maxBulletCount;
	// 총알 UI 가득 채우자.
	InitBulletUI();
}

void ANetTPSCharacter::DamageProcess(float damage)
{
	// HealthBar 클래스 가져오자.
	UHealthBar* hpBar =  Cast<UHealthBar>(compHP->GetWidget());
	// 가져온 클래스에 있는 UpdateHPBar 실행
	hpBar->UpdateHPBar(damage);
}

void ANetTPSCharacter::BillboardHP()
{
	// 내가 컨트롤하고 있는 카메라를 가져오자.
	AActor* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// 카메라의 앞방향(반대), 윗방향을 이용해서 Rotator 를 구하자.
	FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-cam->GetActorForwardVector(), cam->GetActorUpVector());
	// compHP 를 구한  Rotator 값으로 설정.
	compHP->SetWorldRotation(rot);
}

void ANetTPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ANetTPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANetTPSCharacter::TakePistol()
{	
	// 내가 총을 들고 있지 않다면
	if (bHasPistol == false)
	{
		// 바닥에 있는 총을 검색하자.
		TArray<AActor*> allPistols;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APistol::StaticClass(), allPistols);

		// 현재까지 최단거리 (초기 값을 float 의 최대치로)
		float closestDist = std::numeric_limits<float>::max();
		// 현재까지 최단거리의 총
		AActor* closestPistol = nullptr;
		
		for (int32 i = 0; i < allPistols.Num(); i++)
		{
			// 소유자가 있니?
			if(allPistols[i]->GetOwner() != nullptr) continue;

			// 나와 총의 거리를 구하자.
			float dist = FVector::Distance(allPistols[i]->GetActorLocation(), GetActorLocation());

			// 그 거리가 총을 주울 수 있는 범위 밖에 있으면
			if (dist > distanceToGun) continue;			
			
			// 그 거리가 최단거리보다 작으면
			if (dist < closestDist)
			{
				// 최단거리 갱신
				closestDist = dist;
				// 최단거리 총 갱신
				closestPistol = allPistols[i];
			}			
		}

		// 만약에 최단거리 총이 있다면
		if (closestPistol != nullptr)
		{
			// 현재 들고 있는 총 설정
			ownedPistol = Cast<APistol>(closestPistol);
			// 소유자를 나로 설정
			ownedPistol->SetOwner(this);

			// 현재 총을 들고 있는 상태
			bHasPistol = true;

			// 물리적인 움직임을 꺼주자.
			UStaticMeshComponent* comp = ownedPistol->GetComponentByClass<UStaticMeshComponent>();
			comp->SetSimulatePhysics(false);
			// 검색된 총을 compGun 에 자식으로 붙이자.
			ownedPistol->AttachToComponent(compGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// 총 들었을 때 움직이는 설정 변경
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			CameraBoom->TargetArmLength = 150;
			CameraBoom->SetRelativeLocation(FVector(0, 40, 60));

			// crosshair 보이게
			mainUI->ShowCrosshair(true);

			// 총알 갯수 설정
			InitBulletUI();
		}			
	}


	// 내가 총을 들고 있다면
	else
	{
		// 만약에 재장전 중이라면 함수를 나가자.
		if (bReloading) return;
		
		// 물리적인 움직임 켜주자.
		UStaticMeshComponent* comp = ownedPistol->GetComponentByClass<UStaticMeshComponent>();
		comp->SetSimulatePhysics(true);
		// 총을 바닥에 떨어뜨리자.
		ownedPistol->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// 현재 총을 들고 있지 않은 상태
		bHasPistol = false;

		// 소유자를 없게 하자.
		ownedPistol->SetOwner(nullptr);
		// 현재 들고 있는 총을 nullptr 로!
		ownedPistol = nullptr;

		// 총 들었을 때 움직이는 설정 변경
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		CameraBoom->TargetArmLength = 400;
		CameraBoom->SetRelativeLocation(FVector(0, 0, 0));

		// crosshair 보이지 않게
		mainUI->ShowCrosshair(false);
		// 총알 UI 모두 삭제
		mainUI->PopBulletAll();
	}
}

void ANetTPSCharacter::Fire()
{
	// 만약에 총을 들고있지 않으면 함수를 나가자.
	if (bHasPistol == false) return;
	// 만약에 총알갯수가 0이라면 함수를 나가자.
	if (ownedPistol->currBulletCount <= 0) return;
	// 만약에 재장전 중이면 함수를 나가자.
	if (bReloading) return;
	
	// 시작 지점
	FVector startPos = FollowCamera->GetComponentLocation();
	// 종료 지점
	FVector endPos = startPos + FollowCamera->GetForwardVector() * 100000;
	// 그 외 옵션
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	// 부딪혔을 때 그 정보를 담을 변수
	FHitResult hitInfo;
	// LineTrace 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);
	// 만약에 부딪히였다면
	if (bHit)
	{
		// 총알 이펙트 표현
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), gunEffect, hitInfo.Location, FRotator(), true);
		// 만약에 맞은 Actor 가 NetTPSCharacter 라면
		ANetTPSCharacter* player = Cast<ANetTPSCharacter>(hitInfo.GetActor());
		if (player)
		{
			// HP 를 줄이자.
			player->DamageProcess(10);
		}
	}

	// 총 쏘는 애니메이션 실행
	PlayAnimMontage(fireMontage, 2.0f, TEXT("Fire"));
	// 총알 소모
	ownedPistol->currBulletCount--;
	// 총알 UI 하나 제거
	mainUI->PopBullet(ownedPistol->currBulletCount);
}

void ANetTPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// MainUI 만들자
	mainUI = CreateWidget<UMainUI>(GetWorld(), mainUIWidget);
	mainUI->AddToViewport();
}

void ANetTPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 만약에 R 키를 눌렀다면
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::R))
	{
		Reload();
	}
	// 만약에 R 키를 떼었다면
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustReleased(EKeys::R))
	{
		
	}
	// 만약에 R 키를 누르고 있다면
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::R))
	{
		
	}

	// HealtBar 계속 나를 바라보게
	BillboardHP();
}
