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
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage1;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage2;

	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	class UAnimMontage* attackMontage3;

	void PlayAttackAnimation1();
	void PlayAttackAnimation2();
	void PlayAttackAnimation3();

	bool bIsAttacking = false;
};
