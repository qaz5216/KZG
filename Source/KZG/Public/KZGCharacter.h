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

public:
	AKZGCharacter();
	

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
			
	void InputRun();

	void CrouchInput();

	void AttackInput();
 
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

};

