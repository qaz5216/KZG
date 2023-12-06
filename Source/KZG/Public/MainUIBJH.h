// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <UMG/Public/Components/Image.h>
#include <Internationalization/Text.h>
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
	UFUNCTION(BlueprintCallable)
	void SwitchChar(int32 index);
	UFUNCTION(BlueprintCallable)
	void SwitchMenu1();
	UFUNCTION(BlueprintCallable)
	void SwitchMenu2();
	UFUNCTION(BlueprintCallable)
	void SwitchMenu3();
	UFUNCTION(BlueprintCallable)
	void SwitchDay1();
	UFUNCTION(BlueprintCallable)
	void SwitchDay2();
	UFUNCTION(BlueprintCallable)
	void SwitchDay3();
	UFUNCTION(BlueprintCallable)
	void BackIndex();

	UPROPERTY(EditAnywhere, Category = "MyFactory2")
	FSlateBrush ImageArray[3];
	//Switcher
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UWidgetSwitcher* ws_SessionUISwitch;
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UWidgetSwitcher* WidgetSwitcherMenu;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UWidgetSwitcher* WidgetSwitcherDay;


	//Image
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UImage* image_Char;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UImage* image_LastChar;
		//menu
		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
		class UImage* image_Title1;
		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
		class UImage* image_Title2;
		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
		class UImage* image_Title3;
		
		//day
		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
		class UImage* image_Day1;
		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
		class UImage* image_Day2;
		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
		class UImage* image_Day3;
	//Button
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Login;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_AddChar;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_EndSetting;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Menu1;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Menu2;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Menu3;

		UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Day1;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Day2;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category=MySettings)
	class UButton* Button_Day3;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	FText charname;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	FText BackGroundText;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	int32 cur_index=0;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	int32 Char_index=0;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	int32 Menu_index = 0;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category=MySettings)
	int32 Day_index = 0;

	//				0				1				2													3	
	// 필요정보 캔버스인덱스,캐릭터메쉬인덱스, 설정메뉴인덱스(캐릭터추천,배경설정,이벤트설정) , Day(이벤트설정)
	TArray<TArray<int32>> BackElement;
};
