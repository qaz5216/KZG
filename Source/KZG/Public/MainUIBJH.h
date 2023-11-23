// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIBJH.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API UMainUIBJH : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SwitchCanvas(int32 index);

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UWidgetSwitcher* ws_SessionUISwitch;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	int32 cur_index=0;
};
