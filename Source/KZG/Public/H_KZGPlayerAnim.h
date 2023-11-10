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

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* deathMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage2;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage3;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* finalAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* assasinationMontage;

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

	void PlayAttackAnimation2();
	void PlayAttackAnimation3();
	void PlayDeathAnimation();

	void finalAttackAnimation3();
	void playAssasinationAnimation();

	class AKZGCharacter* player;

};
