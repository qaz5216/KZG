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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SeeScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* GrabbedCam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CamFollowComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxComp;
	
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

	
	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<class UCameraShakeBase> ZHitBase;

	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<class UCameraShakeBase> ZGrabbedBase;

	UPROPERTY(EditDefaultsOnly, Category="CameraShake")
	TSubclassOf<class UCameraShakeBase> ZFinalBase;

	UPROPERTY(EditDefaultsOnly, Category="PlaySound")
	class USoundBase* batHitSound;

public:
	AKZGCharacter();
	

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
			
	UFUNCTION(Server, Reliable)
	void Server_InputRun();

	UFUNCTION(Server, Reliable)
	void Server_CrouchInput();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CrouchInput();

	UFUNCTION(Server, Reliable)
	void Server_AttackInput();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttackInput();

	UFUNCTION(Server, Reliable)
	void Server_InteractionInput();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InteractionUnput();

	UFUNCTION(Server, Reliable)
	void Server_InteractionInputEnd();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_InteractionUnputEnd();

	UFUNCTION(Server, Reliable)
	void Server_ChangeView();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeView();

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	float walkSpeed = 300;
	// ¶Ù±â ¼Óµµ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	float runSpeed = 800;
	// ´Ù½Ã ¹Ù²ð¼Óµµ
	float returnSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings" , Replicated)
	bool bIsCrouching = false;

	UPROPERTY(Replicated)
	bool bIsRunning = false;

	UPROPERTY(Replicated)
	bool bOnDamaged = false;

	UPROPERTY(Replicated)
	bool bIsAttacking = false;

	UPROPERTY(Replicated)
	bool bIsFinalAttackEnded = false;

	UPROPERTY(Replicated)
	bool bIsInteractionInput = false;

	UPROPERTY(Replicated)
	int32 playerStamina = 500;

	UPROPERTY(Replicated)
	int32 currentStamina = 0;

	UPROPERTY(Replicated)
	int32 maxHungerP = 100;

	UPROPERTY(Replicated)
	int32 curHungerP = 0;

	float camArmLen;

	UPROPERTY(Replicated)
	float curHungtime = 0;

	void PlayStepSoundPlaying();

	float stepSoundrad=1000;

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsgrabbed = false;

	UPROPERTY(Replicated)
	bool bCangrabbed = true;

	void GrabbedbyZombie(class AEnemy *Enemy);

	void EscapebyZombie();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MySettings)
	float grabbedDelayTime = 5.0f;

	void TryEscape();

	void GrabbedDelay();

	UPROPERTY(Replicated)
	float recoverTime = 3;

	UPROPERTY(Replicated)
	float curSP = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MySettings)
	int32 recoveryPoint = 5;

	float lerpMaxTime = 1;

	float lerpCurTime;

	bool bIsOverlapping = false;

	UPROPERTY(EditAnywhere)
	int32 damagePower = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class AEnemy *GrabbedEnemy;

public:
	void DamagedStamina(int32 value);

	UFUNCTION(Server, Reliable)
	void Server_GrabbedWidget();

	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_GrabbedWidget();

	FVector CameraLocation;
	FRotator CameraRot;

	FVector CameraMoveLoc = FVector(0.000000, 260.000000, 0.000000);
	FRotator CameraMoveRot = FRotator(0.000000, -50.000000,  0.000000);

	FTimerHandle BlendTimerHandle;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

