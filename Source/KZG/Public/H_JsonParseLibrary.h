// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "H_JsonParseLibrary.generated.h"

/**
 * 
 */
UCLASS()
class KZG_API UH_JsonParseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FString JsonParse(const FString& originData);

	static FString MakeJson(const TMap<FString, FString> source);
};
