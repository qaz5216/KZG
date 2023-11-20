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
	class UStaticMeshComponent* gunMesh;

	/*UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxComp;*/

	//UPROPERTY(VisibleAnywhere)
	//class UStaticMeshComponent* batMesh;
	//
	//UPROPERTY(VisibleAnywhere)
	//class UStaticMeshComponent* axeMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* throwAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* pressedOne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* pressedTwo;

public:
	UPROPERTY(VisibleAnywhere, Category = MySettings, Replicated, meta = (AllowPrivateAccess = "true"))
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

	

	UPROPERTY(EditDefaultsOnly, Category = "swingSound")
	class USoundBase* batSwingSound;

	UPROPERTY(EditDefaultsOnly, Category="AssasinationSound")
	class USoundBase* assasinationSound;

	UPROPERTY(EditDefaultsOnly, Category="BGMSound")
	class USoundBase* bgmSound;

	UPROPERTY(EditDefaultsOnly, Category="gunShotSound")
	class USoundBase* gunShotSound;

	//UPROPERTY(EditDefaultsOnly, Category="audioComp")
	//class UAudioComponent* audioComp;

	//UPROPERTY(EditDefaultsOnly, Category = "Audio")
 //   class USoundCue* DefaultBGM; // DefaultBGM 변수를 추가합니다.

	/*void SwitchBGMtoDetecting();
	void SwitchBGMtoDefault();
	void SwitchBGMtoDiscover();
	void SwitchBGMtoGrab();*/

	/*bool bIsBGMDetecting = false;
	bool bIsBGMDefault = true;
	bool bIsBGMDiscover = false;
	bool bIsBGMGrab = false;*/

public:
	AKZGCharacter();
	

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
			
	UFUNCTION(Server, Reliable)
	void Server_InputRun();

	UFUNCTION(Server, Reliable)
	void Server_CrouchInput();

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

	UFUNCTION()
	void ThrowAction();

	void JumpInput();

	void PressedOneAction();

	void PressedTwoAction();
 
	class UH_KZGPlayerAnim* anim;
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// 걷기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	float walkSpeed = 300;
	// 뛰기 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MySettings")
	float runSpeed = 600;
	// 다시 바뀔속도
	float returnSpeed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	int32 curComboSec = 3;

	int32 comboIndex = 0;

	bool bComboTime = false;

	float curComTime = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	int32 damageNumber = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings" , Replicated)
	bool bIsCrouching = false;

	UPROPERTY(Replicated)
	bool bIsRunning = false;

	UPROPERTY(Replicated)
	bool bOnDamaged = false;

	UPROPERTY(Replicated)
	bool bIsAttacking = false;	UPROPERTY(Replicated)
	bool bIsFinalAttackEnded = false;

	UPROPERTY(Replicated)
	bool bIsInteractionInput = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bCanAssasination=false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bStartAssaination=false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bHasWeapon = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bHasGun = false;

	UPROPERTY(EditAnywhere, Category="AssasinationDistance")
	float assasinationDistance = 50;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Replicated, Category="StaminaSize")
	int32 playerStamina = 500;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Replicated, Category="StaminaSize")
	int32 currentStamina = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated, Category = "StaminaSize")
	int32 maxHungerP = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Replicated, Category = "StaminaSize")
	int32 curHungerP = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "weaponHP")
	int32 realWeaponHP = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "weaponHP")
	int32 weaponDamage = 5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "weaponHP")
	int32 pursuitEnemy = 0;

	float camArmLen;

	UPROPERTY(Replicated)
	float curHungtime = 0;

	void PlayStepSoundPlaying();

	float stepSoundrad=1000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Grabbed")
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


	UPROPERTY(EditAnywhere)
	int32 damagePower = 35;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class AEnemy *GrabbedEnemy;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Replicated, Category="StaminaSize")
	int32 maxsize = 500;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Replicated, Category="StaminaSize")
	class AEnemy *AssaionateEnemy;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Replicated, Category="Weapons")
	TSubclassOf<class AH_AttackWeapons> BP_BatWeapon; 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite ,Replicated, Category="Weapons")
	TSubclassOf<class AH_AttackWeapons> BP_AxeWeapon;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<class ABP_H_Gun> BP_Gun;

	class AH_AttackWeapons* attackWeapon;
	class ABP_H_Gun* gunWeapon;
	UPROPERTY(EditAnywhere, Category="GunDamage")
	int32 gunDamage = 10;

	int32 killNum = 0;
public:
	void DamagedStamina(int32 value);

	UFUNCTION(Server, Reliable)
	void Server_GrabbedWidget();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GrabbedWidget();


	UFUNCTION(Server, Reliable)
	void Server_PlayerDeath();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayerDeath();

	void AttackCollisionOff();
	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_GrabbedWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Death")
	bool bIsDead = false;

	FVector CameraLocation;
	FRotator CameraRot;

	FVector CameraMoveLoc = FVector(0.000000, 260.000000, 0.000000);
	FRotator CameraMoveRot = FRotator(0.000000, -50.000000,  0.000000);

	FTimerHandle BlendTimerHandle;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentBeginOverlapFood(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SetLocation")
	float restartLoc = 0.1f;

	void SetPlayerLocationToFirst();
};

