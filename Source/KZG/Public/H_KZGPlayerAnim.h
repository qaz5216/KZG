// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "H_KZGPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API UH_KZGPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float speed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float direction = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	bool isInAir = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	FRotator deltaRot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	float deltaPitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	float deltaYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	float deltaRoll;


	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* deathMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* deathGrabMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage2;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage3;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* finalAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* assasinationMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* pushMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* assasinationDeathMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* comboMontage1;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* comboMontage2;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* comboMontage3;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* gunShootMon;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* reloadMon;

	UPROPERTY(EditDefaultsOnly, Category="StepSound")
	USoundBase* stepSound;

	UFUNCTION()
	void AnimNotify_AttackEnd1();
	UFUNCTION()
	void AnimNotify_AttackEnd2();
	UFUNCTION()
	void AnimNotify_AttackEnd3();
	UFUNCTION()
	void AnimNotify_SoundStep();
	UFUNCTION()
	void AnimNotify_AssasinationEnd();
	UFUNCTION()
	void AnimNotify_ReloadEnd();

	UFUNCTION()
	void AnimNotify_Combo1();
	UFUNCTION()
	void AnimNotify_Combo2();
	UFUNCTION()
	void AnimNotify_Combo3();

	UFUNCTION()
	void AnimNotify_Combo1End();
	UFUNCTION()
	void AnimNotify_Combo2End();
	UFUNCTION()
	void AnimNotify_Combo3End();

	void PlayAttackAnimation2();
	void PlayAttackAnimation3();
	void PlayDeathAnimation();

	void PlayComboAnimation1();
	void PlayComboAnimation2();
	void PlayComboAnimation3();

	void finalAttackAnimation3();
	void playAssasinationAnimation();
	void PlayDeathGrabMontage();
	void playAssasinationDeathAnimation();
	void playPushAnimation();

	void playGunShootAnim();
	void playReloadAnim();


	class AKZGCharacter* player;

};
