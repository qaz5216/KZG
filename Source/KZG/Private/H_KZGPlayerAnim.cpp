// Fill out your copyright notice in the Description page of Project Settings.


#include "H_KZGPlayerAnim.h"

void UH_KZGPlayerAnim::PlayAttackAnimation1()
{
	Montage_Play(attackMontage1);
}

void UH_KZGPlayerAnim::PlayAttackAnimation2()
{
	Montage_Play(attackMontage2);
}

void UH_KZGPlayerAnim::PlayAttackAnimation3()
{
	Montage_Play(attackMontage3);
}
