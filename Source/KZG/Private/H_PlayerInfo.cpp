// Fill out your copyright notice in the Description page of Project Settings.


#include "H_PlayerInfo.h"
#include "KZGCharacter.h"
#include "Components/ProgressBar.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextBlock.h"
#include <UMG/Public/Components/CanvasPanelSlot.h>
#include <UMG/Public/Components/Image.h>


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
		// ���� ���� ���¹̳� �� �ݿ�(���� ü�� / �ִ� ü��)
		PB_CurStamina->SetPercent((float)player->currentStamina / (float)player->playerStamina);

		UCanvasPanelSlot* MySlot =Cast<UCanvasPanelSlot>(PB_CurStamina->Slot);

		staminaCurTime += InDeltaTime;
		if (player->curHungerP < 95 && player->curHungerP > 70)
		{
			if (staminaCurTime > 5)
			{
				player->maxsize -= 3;
				MySlot->SetSize(FVector2D(player->maxsize,15));
				staminaCurTime = 0;
				//MySlot->SetSize(MySlot->GetSize() - FVector2D(5, 0));
			}
		}
		else if (player->curHungerP > 30 && player->curHungerP <= 70)
		{
			if (staminaCurTime > 5)
			{
				player->maxsize -= 5;
				MySlot->SetSize(FVector2D(player->maxsize, 15));
				staminaCurTime = 0;
				//MySlot->SetSize(MySlot->GetSize() - FVector2D(5, 0));
			}
		}
		else if (player->curHungerP <= 30)
		{
			if (staminaCurTime > 5)
			{
				player->maxsize -= 7;
				MySlot->SetSize(FVector2D(player->maxsize, 15));
				staminaCurTime = 0;
				//MySlot->SetSize(MySlot->GetSize() - FVector2D(5, 0));
			}
		}

		if (player->curHungerP < 30) Image_Hungry->SetVisibility(ESlateVisibility::Visible);
		else if(player->curHungerP >= 30) Image_Hungry->SetVisibility(ESlateVisibility::Hidden);
		if (player->currentStamina < 50) Image_Sleep->SetVisibility(ESlateVisibility::Visible);
		else if (player->currentStamina >= 50) Image_Sleep->SetVisibility(ESlateVisibility::Hidden);

	}
}

