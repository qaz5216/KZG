// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "KZGCharacter.generated.h"


UCLASS(config=Game)
class AKZGCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InterAction;

public:
	UPROPERTY(VisibleAnywhere, Category = MySettings, meta = (AllowPrivateAccess = "true"))
	class UH_EWidget* EWidget;
	
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UH_EWidget> BP_EWidget;

	UPROPERTY(VisibleAnywhere, Category = MySettings, meta = (AllowPrivateAccess = "true"))
	class UH_PlayerInfo* InfoWidget;
	
	UPROPERTY(EditAnywhere, Category = MySettings)
	TSubclassOf<class UH_PlayerInfo> BP_InfoWidget;
public:
	AKZGCharacter();
	

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
			
	void InputRun();

	void CrouchInput();

	void AttackInput();

	void InteractionInput();

	void JumpInput();
 
	class UH_KZGPlayerAnim* anim;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// °È±â ¼Óµµ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float walkSpeed = 300;
	// ¶Ù±â ¼Óµµ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	float runSpeed = 800;
	// ´Ù½Ã ¹Ù²ð¼Óµµ
	float returnSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings")
	bool bIsCrouching = false;

	bool bIsRunning = false;

	bool bOnDamaged = false;

	bool bIsAttacking = false;

	int32 playerStamina = 40;

	int32 currentStamina = 0;

	void PlayStepSoundPlaying();

	float stepSoundrad=1000;

	bool bIsgrabbed = false;

	void GrabbedbyZombie(class AEnemy *Enemy);

	void EscapebyZombie();

	void TryEscape();

	float recoverTime = 3;

	float curSP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MySettings)
	int32 recoveryPoint = 2;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class AEnemy *GrabbedEnemy;

public:
	void DamagedStamina(int32 value);
};

