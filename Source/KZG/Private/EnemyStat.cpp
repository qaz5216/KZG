// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStat.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/CanvasPanel.h>
#include <UMG/Public/Components/TextBlock.h>
#include <UMG/Public/Components/ProgressBar.h>

void UEnemyStat::NativeConstruct()
{
	Super::NativeConstruct();
	ETextImg->SetVisibility(ESlateVisibility::Hidden);
	Canvas1->SetVisibility(ESlateVisibility::Hidden);
}

void UEnemyStat::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UEnemyStat::EImageHidden()
{
	ETextImg->SetVisibility(ESlateVisibility::Hidden);
}

void UEnemyStat::EImageShow()
{
	ETextImg->SetVisibility(ESlateVisibility::Visible);
}

void UEnemyStat::HiddenUI()
{
	Canvas1->SetVisibility(ESlateVisibility::Hidden);
}

void UEnemyStat::ShowUI()
{
	Canvas1->SetVisibility(ESlateVisibility::Visible);
}


void UEnemyStat::SetMaxHP(float val)
{
	int32 newval=val;
	MaxHP_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), newval)));
}

void UEnemyStat::SetHPBar(float val,float max)
{
	int32 newval=val;
	UE_LOG(LogTemp,Warning,TEXT("%d"),newval);
	CurHP_Text->SetText(FText::FromString(FString::Printf(TEXT("%d"), newval)));
	pb_healthBar->SetPercent(val/max);
}

void UEnemyStat::SetStaminaBar(float val, float max)
{
	pb_staminaBar->SetPercent(val / max);
}
