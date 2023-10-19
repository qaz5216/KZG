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
	class UAnimMontage* attackMontage1;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage2;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage3;


	// DieEndNoti 이벤트가 들어오면 그때서야 DieState 가 동작하도록 하고싶다.
	UFUNCTION()
	void AnimNotify_AttackEnd1();

	void PlayAttackAnimation1();
	void PlayAttackAnimation2();
	void PlayAttackAnimation3();

	class AKZGCharacter* player;

	bool bIsAttacking = false;
};
