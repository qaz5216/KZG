// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"

void UEnemyAnimInstance::OnAttackEndEvent()
{
	bAttackPlay = false;
}

void UEnemyAnimInstance::AnimNotify_DieEndNoti()
{
	// 이제 DieState 함수 재생해라.
	isDiePlaying = true;
}
