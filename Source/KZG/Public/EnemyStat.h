// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStat.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API UEnemyStat : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UCanvasPanel* Canvas1;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UProgressBar* pb_healthBar;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UProgressBar* pb_staminaBar;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* CurHP_Text;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* MaxHP_Text;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UImage* ETextImg;
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void EImageHidden();
	void EImageShow();
	void HiddenUI();
	void ShowUI();
	void SetMaxHP(float val);
	void SetHPBar(float val, float max);
	void SetStaminaBar(float val, float max);
private:
	class AEnemy* Enemy;
};
