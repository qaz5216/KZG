// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBJH.h"
#include <UMG/Public/Components/WidgetSwitcher.h>
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/Button.h>

void UMainUIBJH::NativeConstruct()
{
	Button_Menu1->OnClicked.AddDynamic(this, &UMainUIBJH::SwitchMenu1);
	Button_Menu2->OnClicked.AddDynamic(this, &UMainUIBJH::SwitchMenu2);
	Button_Menu3->OnClicked.AddDynamic(this, &UMainUIBJH::SwitchMenu3);
	Button_Day1->OnClicked.AddDynamic(this, &UMainUIBJH::SwitchDay1);
	Button_Day2->OnClicked.AddDynamic(this, &UMainUIBJH::SwitchDay2);
	Button_Day3->OnClicked.AddDynamic(this, &UMainUIBJH::SwitchDay3);
}

void UMainUIBJH::SwitchCanvas(int32 index)
{
	TArray<int32> tmpele;
	tmpele.Add(cur_index);
	tmpele.Add(Char_index);
	tmpele.Add(Menu_index);
	tmpele.Add(Day_index);
	BackElement.Add(tmpele);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Add:%d,%d,%d,%d"), tmpele[0], tmpele[1], tmpele[2], tmpele[3]));
	cur_index=index;
	//캔버스바꾸기
	ws_SessionUISwitch->SetActiveWidgetIndex(index);
}

void UMainUIBJH::SwitchChar(int32 index)
{
	TArray<int32> tmpele;
	tmpele.Add(cur_index);
	tmpele.Add(Char_index);
	tmpele.Add(Menu_index);
	tmpele.Add(Day_index);
	BackElement.Add(tmpele);

	Char_index = index;
	//캐릭터메쉬바꾸기
	image_Char->SetBrush(ImageArray[Char_index]);
	image_LastChar->SetBrush(ImageArray[Char_index]);
}

void UMainUIBJH::SwitchMenu1()
{
	int32 index=0;
	if (Menu_index != index)
	{
		TArray<int32> tmpele;
		tmpele.Add(cur_index);
		tmpele.Add(Char_index);
		tmpele.Add(Menu_index);
		tmpele.Add(Day_index);
		BackElement.Add(tmpele);

		Menu_index = index;
		//메뉴바꾸기
		WidgetSwitcherMenu->SetActiveWidgetIndex(index);
		image_Title1->SetVisibility(ESlateVisibility::Visible);
		image_Title2->SetVisibility(ESlateVisibility::Hidden);
		image_Title3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUIBJH::SwitchMenu2()
{
	int32 index = 1;
	if (Menu_index != index)
	{
		TArray<int32> tmpele;
		tmpele.Add(cur_index);
		tmpele.Add(Char_index);
		tmpele.Add(Menu_index);
		tmpele.Add(Day_index);
		BackElement.Add(tmpele);

		Menu_index = index;
		//메뉴바꾸기
		WidgetSwitcherMenu->SetActiveWidgetIndex(index);
		image_Title1->SetVisibility(ESlateVisibility::Hidden);
		image_Title2->SetVisibility(ESlateVisibility::Visible);
		image_Title3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUIBJH::SwitchMenu3()
{
	int32 index = 2;
	if (Menu_index != index)
	{
		TArray<int32> tmpele;
		tmpele.Add(cur_index);
		tmpele.Add(Char_index);
		tmpele.Add(Menu_index);
		tmpele.Add(Day_index);
		BackElement.Add(tmpele);

		Menu_index = index;
		//메뉴바꾸기
		WidgetSwitcherMenu->SetActiveWidgetIndex(index);
		image_Title1->SetVisibility(ESlateVisibility::Hidden);
		image_Title2->SetVisibility(ESlateVisibility::Hidden);
		image_Title3->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainUIBJH::SwitchDay1()
{
	int32 index=0;
	if (Day_index!=index)
	{
		TArray<int32> tmpele;
		tmpele.Add(cur_index);
		tmpele.Add(Char_index);
		tmpele.Add(Menu_index);
		tmpele.Add(Day_index);
		BackElement.Add(tmpele);
		Day_index = index;
		//Day바꾸기
		WidgetSwitcherDay->SetActiveWidgetIndex(index);
		image_Day1->SetVisibility(ESlateVisibility::Visible);
		image_Day2->SetVisibility(ESlateVisibility::Hidden);
		image_Day3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUIBJH::SwitchDay2()
{
	int32 index = 1;
	if (Day_index != index)
	{	
		TArray<int32> tmpele;
		tmpele.Add(cur_index);
		tmpele.Add(Char_index);
		tmpele.Add(Menu_index);
		tmpele.Add(Day_index);
		BackElement.Add(tmpele);
		Day_index = index;
		//Day바꾸기
		WidgetSwitcherDay->SetActiveWidgetIndex(index);
		image_Day1->SetVisibility(ESlateVisibility::Hidden);
		image_Day2->SetVisibility(ESlateVisibility::Visible);
		image_Day3->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainUIBJH::SwitchDay3()
{
	int32 index = 2;
	if (Day_index != index)
	{
		TArray<int32> tmpele;
		tmpele.Add(cur_index);
		tmpele.Add(Char_index);
		tmpele.Add(Menu_index);
		tmpele.Add(Day_index);
		BackElement.Add(tmpele);
		Day_index = index;
		//Day바꾸기
		WidgetSwitcherDay->SetActiveWidgetIndex(index); 
		image_Day1->SetVisibility(ESlateVisibility::Hidden);
		image_Day2->SetVisibility(ESlateVisibility::Hidden);
		image_Day3->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainUIBJH::BackIndex()
{
	if (!BackElement.IsEmpty())
	{
		TArray<int32> backele=BackElement.Pop();
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Back:%d,%d,%d,%d"),backele[0],backele[1],backele[2],backele[3]));
		if (cur_index!=backele[0])
		{
			cur_index = backele[0];
			// 캔버스 바꾸기
			ws_SessionUISwitch->SetActiveWidgetIndex(cur_index);
		}
		if (Char_index != backele[1])
		{
			Char_index = backele[1];
			// 캐릭터 바꾸기
			image_Char->SetBrush(ImageArray[Char_index]);
		}
		if (Menu_index != backele[2])
		{
			Menu_index = backele[2];
			// 메뉴 바꾸기
			if (Menu_index==0)
			{
				image_Title1->SetVisibility(ESlateVisibility::Visible);
				image_Title2->SetVisibility(ESlateVisibility::Hidden);
				image_Title3->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (Menu_index==1)
			{
				image_Title1->SetVisibility(ESlateVisibility::Hidden);
				image_Title2->SetVisibility(ESlateVisibility::Visible);
				image_Title3->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (Menu_index == 2)
			{
				image_Title1->SetVisibility(ESlateVisibility::Hidden);
				image_Title2->SetVisibility(ESlateVisibility::Hidden);
				image_Title3->SetVisibility(ESlateVisibility::Visible);
			}
			WidgetSwitcherMenu->SetActiveWidgetIndex(Menu_index);
		}
		if (Day_index != backele[3])
		{
			Day_index = backele[3];
			// Day 바꾸기
			WidgetSwitcherDay->SetActiveWidgetIndex(Day_index);
			if (Day_index==0)
			{
				image_Day1->SetVisibility(ESlateVisibility::Visible);
				image_Day2->SetVisibility(ESlateVisibility::Hidden);
				image_Day3->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (Day_index==1)
			{
				image_Day1->SetVisibility(ESlateVisibility::Hidden);
				image_Day2->SetVisibility(ESlateVisibility::Visible);
				image_Day3->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (Day_index == 2)
			{
				image_Day1->SetVisibility(ESlateVisibility::Hidden);
				image_Day2->SetVisibility(ESlateVisibility::Hidden);
				image_Day3->SetVisibility(ESlateVisibility::Visible);
			}
			
		}
	}
}
