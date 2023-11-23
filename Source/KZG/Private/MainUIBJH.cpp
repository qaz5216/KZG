// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBJH.h"
#include <UMG/Public/Components/WidgetSwitcher.h>

void UMainUIBJH::NativeConstruct()
{

}

void UMainUIBJH::SwitchCanvas(int32 index)
{
	
	ws_SessionUISwitch->SetActiveWidgetIndex(index);
}
