// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::OnAttackEndEvent()
{
	bAttackPlay = false;
}

void UEnemyAnimInstance::AnimNotify_DieEndNoti()
{
	// ���� DieState �Լ� ����ض�.
	isDiePlaying = true;
}
