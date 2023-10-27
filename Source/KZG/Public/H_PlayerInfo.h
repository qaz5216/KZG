// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "H_PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API UH_PlayerInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Replicated)
	class UProgressBar* PB_CurStamina;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Replicated)
	class UTextBlock* text_Hungry;
	
	UPROPERTY(Replicated)
	float staminaCurTime = 0;

	UPROPERTY(Replicated)
	float maxsize = 1;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	class AKZGCharacter* player;
};
