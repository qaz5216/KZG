// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../EnemyFsm.h"

#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// ���� ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	bool bAttackPlay = false;

	// ���� ���� �̺�Ʈ�� ȣ��Ǹ� ó���� �Լ�
	UFUNCTION(BlueprintCallable, Category="FSM")
	void OnAttackEndEvent();

	// �ǰ� ���� ��ȯ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bDamage = false;

	// �ǰ� ��Ÿ�� ó���Լ� ����
	UFUNCTION(BlueprintImplementableEvent, Category="FSM")
	void PlayDamageAnim(FName sectionName);
	
	UFUNCTION(BlueprintImplementableEvent, Category="FSM")
	void PlayStaminaAnim(FName sectionName);
	
	UFUNCTION(BlueprintImplementableEvent, Category="FSM")
	void PlayDieAnim();
	

	// DieEndNoti �̺�Ʈ�� ������ �׶����� DieState �� �����ϵ��� �ϰ�ʹ�.
	UFUNCTION()
	void AnimNotify_DieEndNoti();

	// DieState �����ص� �Ǵ��� ����
	bool isDiePlaying = false;
};
