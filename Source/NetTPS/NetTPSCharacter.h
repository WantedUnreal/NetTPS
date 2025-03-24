// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NetTPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ANetTPSCharacter : public ACharacter
{

	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TakePistolAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

public:
	ANetTPSCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void TakePistol();
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakePistol();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_AttachPistol(class APistol* pistol);
	UFUNCTION()
	void AttachPistol();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DetachPistol(class APistol* pistol);

	
	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Fire(bool bHit, FHitResult hitInfo);
			

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PossessedBy(AController* NewController) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// 총을 붙일 컴포넌트
	UPROPERTY(EditAnywhere)
	class USceneComponent* compGun;

	// HealthBar  붙일 컴포넌트
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* compHP;

	// 현재 총을 들고 있는지 여부
	bool bHasPistol = false;

	// 현재 재장전 중인지 여부
	bool bReloading = false;
	
	// 어떤 총을 들고 있는지
	UPROPERTY(ReplicatedUsing=AttachPistol)
	class APistol* ownedPistol = nullptr;

	// 총을 집을 수 있는 범위
	UPROPERTY(EditAnywhere)
	float distanceToGun = 200;

	// 총알 피격 이펙트
	UPROPERTY(EditAnywhere)
	class UParticleSystem* gunEffect;

	// 총 관련 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontage;

	// MainUI 위젯
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> mainUIWidget;
	UPROPERTY()
	class UMainUI* mainUI;

	// 총알 UI 셋팅
	void InitBulletUI();

	// 총알 재장전 시작
	void Reload();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Reload();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Reload();

	// 총알 재장전 완료
	void ReloadComplete();

	// 피격 당했을 때 호출되는 함수
	void DamageProcess(float damage);
	// 죽었을 때 호출되는 함수
	void DieProcess();
	// HPBar billboard 처리
	void BillboardHP();

	// 죽었니?
	bool isDead = false;

	// 빙의 됐을때 알려주는 함수
	UFUNCTION(Client, Reliable)
	void ClientRPC_InitMainUI();

	void PrintNetLog();

	// cube 블루프린트 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> cubeFactory;
	// 큐브 만드는 함수
	void MakeCube();
	UFUNCTION(Server, Reliable)
	void ServerRPC_MakeCube();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_MakeCube(FVector pos, FRotator rot);

	// Turn 기능 관련
	UPROPERTY(Replicated)
	bool canMakeCube = false;
};

