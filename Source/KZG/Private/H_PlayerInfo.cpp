// Fill out your copyright notice in the Description page of Project Settings.


#include "H_PlayerInfo.h"
#include "KZGCharacter.h"
#include "Components/ProgressBar.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextBlock.h"
#include <UMG/Public/Components/CanvasPanelSlot.h>


void UH_PlayerInfo::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<AKZGCharacter>(GetOwningPlayerPawn());
}

void UH_PlayerInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (player != nullptr)
	{
		// 소유 폰의 스태미너 값 반영(현재 체력 / 최대 체력)
		PB_CurStamina->SetPercent((float)player->currentStamina / (float)player->playerStamina);

		UCanvasPanelSlot* MySlot =Cast<UCanvasPanelSlot>(PB_CurStamina->Slot);

		staminaCurTime += InDeltaTime;
		if (player->curHungerP < 95 && player->curHungerP > 70)
		{
			if (staminaCurTime > 5)
			{
				player->maxsize -= 5;
				MySlot->SetSize(FVector2D(player->maxsize,50));
				staminaCurTime = 0;
				//MySlot->SetSize(MySlot->GetSize() - FVector2D(5, 0));
			}
		}
		else if (player->curHungerP > 30 && player->curHungerP <= 70)
		{
			if (staminaCurTime > 5)
			{
				player->maxsize -= 7;
				MySlot->SetSize(FVector2D(player->maxsize, 50));
				staminaCurTime = 0;
				//MySlot->SetSize(MySlot->GetSize() - FVector2D(5, 0));
			}
		}
		else if (player->curHungerP <= 30)
		{
			if (staminaCurTime > 5)
			{
				player->maxsize -= 10;
				MySlot->SetSize(FVector2D(player->maxsize, 50));
				staminaCurTime = 0;
				//MySlot->SetSize(MySlot->GetSize() - FVector2D(5, 0));
			}
		}

		if (player->curHungerP < 30) text_Hungry->SetVisibility(ESlateVisibility::Visible);
		else if(player->curHungerP >= 30) text_Hungry->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UH_PlayerInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UH_PlayerInfo, text_Hungry);
	DOREPLIFETIME(UH_PlayerInfo, staminaCurTime);
}